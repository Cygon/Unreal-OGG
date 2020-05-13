#pragma region MIT License
/*
OggAsset Library
Copyright (C) 2017-2018 Scott Bishel

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/
#pragma endregion // MIT License

#include "OggAssetErrors.h"
#include "SoundProcessingLibrary.h"

#include "OggAsset.h"

#include <Sound/SoundWave.h>
#include <AudioDevice.h>
#include <Runtime/Engine/Public/VorbisAudioInfo.h>
#include <Developer/TargetPlatform/Public/Interfaces/IAudioFormat.h>

/// Functions to load Data from the HardDrive

USoundWave* USoundProcessingLibrary::LoadOggFile(const FString& InFilePath)
{
    // TArray that holds the binary and encoded Sound data
  TArray<uint8> RawFile;

  // Load file into RawFile Array
  bool bLoaded = FFileHelper::LoadFileToArray(RawFile, InFilePath.GetCharArray().GetData());

  if (!bLoaded)
    return nullptr;

  return USoundProcessingLibrary::LoadData(RawFile);
}

USoundWave* USoundProcessingLibrary::LoadData(const TArray<uint8>& RawFile)
{
  // Create new SoundWave Object
  USoundWave* CompressedSoundWaveRef = NewObject<USoundWave>(USoundWave::StaticClass());

  // Make sure the SoundWave Object is Valid
  if (!CompressedSoundWaveRef) {
    UE_LOG(
      LogOggAsset, Error,
      TEXT("%s - %s"),
      TEXT("USoundProcessingLibrary::LoadData()"),
      TEXT("Failed to create a new SoundWave object!")
    );
    return nullptr;
  }

  // Fill the SoundData into the SoundWave Object
  if (RawFile.Num() > 0) {

    if (!FillSoundWaveInfo(CompressedSoundWaveRef, (TArray<uint8>*)&RawFile)) {
    UE_LOG(
      LogOggAsset, Error,
      TEXT("%s - %s"),
      TEXT("USoundProcessingLibrary::LoadData()"),
      TEXT("Could not read compressed vorbis audio stream. Asset corrupted?")
    );
      return nullptr;
    }
  }	else {
    UE_LOG(
      LogOggAsset, Error,
      TEXT("%s - %s"),
      TEXT("USoundProcessingLibrary::LoadData()"),
      TEXT("Raw file input array is empty. Sound not loaded/assigned?")
    );
    return nullptr;
  }
    

  // Get Pointer to the Compressed OGG Data
  FByteBulkData* BulkData = &CompressedSoundWaveRef->CompressedFormatData.GetFormat(FName("OGG"));

  // Set the Lock of the BulkData to ReadWrite
  BulkData->Lock(LOCK_READ_WRITE);

  // Copy compressed RawFile Data to the Address of the OGG Data of the SW File
  FMemory::Memmove(BulkData->Realloc(RawFile.Num()), RawFile.GetData(), RawFile.Num());

  // Unlock the BulkData again
  BulkData->Unlock();

  return CompressedSoundWaveRef;
}

void USoundProcessingLibrary::LoadSoundWave(USoundWave* CompressedSoundWaveRef, const TArray<uint8>& RawFile)
{
  // Make sure the SoundWave Object is Valid
  if (!CompressedSoundWaveRef) {
    UE_LOG(
      LogOggAsset, Error,
      TEXT("%s - %s"),
      TEXT("USoundProcessingLibrary::LoadSoundWave()"),
      TEXT("Compressed sound wave instance was null")
    );
    return;
  }

  // Fill the SoundData into the SoundWave Object
  if (RawFile.Num() > 0) {
    if (!FillSoundWaveInfo(CompressedSoundWaveRef, (TArray<uint8>*)&RawFile)) {
      UE_LOG(
        LogOggAsset, Error,
        TEXT("%s - %s"),
        TEXT("USoundProcessingLibrary::LoadSoundWave()"),
        TEXT("Could not read compressed vorbis audio stream. Asset corrupted?")
      );
      return;
    }
  }	else {
    UE_LOG(
      LogOggAsset, Error,
      TEXT("%s - %s"),
      TEXT("USoundProcessingLibrary::LoadSoundWave()"),
      TEXT("Raw file input array is empty. Sound not loaded/assigned?")
    );
    return;
  }

  // Get Pointer to the Compressed OGG Data
  FByteBulkData* BulkData = &CompressedSoundWaveRef->CompressedFormatData.GetFormat(FName("OGG"));

  // Set the Lock of the BulkData to ReadWrite
  BulkData->Lock(LOCK_READ_WRITE);

  // Copy compressed RawFile Data to the Address of the OGG Data of the SW File
  FMemory::Memmove(BulkData->Realloc(RawFile.Num()), RawFile.GetData(), RawFile.Num());

  // Unlock the BulkData again
  BulkData->Unlock();
}


bool USoundProcessingLibrary::FillSoundWaveInfo(USoundWave* InSoundWave, TArray<uint8>* InRawFile)
{
  // Info Structs
  FSoundQualityInfo SoundQualityInfo;
  FVorbisAudioInfo VorbisAudioInfo;

  // Save the Info into SoundQualityInfo
  if (!VorbisAudioInfo.ReadCompressedInfo(InRawFile->GetData(), InRawFile->Num(), &SoundQualityInfo))
  {
    return false;
  }

  // Fill in all the Data we have
  InSoundWave->DecompressionType = EDecompressionType::DTYPE_RealTime;
  InSoundWave->SoundGroup = ESoundGroup::SOUNDGROUP_Default;
  InSoundWave->NumChannels = SoundQualityInfo.NumChannels;
  InSoundWave->Duration = SoundQualityInfo.Duration;
  InSoundWave->RawPCMDataSize = SoundQualityInfo.SampleDataSize;
  InSoundWave->SampleRate = SoundQualityInfo.SampleRate;

  return true;
}

/// Function to decompress the compressed Data that comes with the .ogg file

void USoundProcessingLibrary::GetPCMDataFromFile(USoundWave* InSoundWave)
{
  if (InSoundWave == nullptr)	{
    UE_LOG(
      LogOggAsset, Error,
      TEXT("%s - %s"),
      TEXT("USoundProcessingLibrary::GetPCMDataFromFile()"),
      TEXT("Passed sound wave is null")
    );
    return;
  }

  if (InSoundWave->NumChannels < 1 || InSoundWave->NumChannels > 2) {
    UE_LOG(
      LogOggAsset, Error,
      TEXT("%s - %s"),
      TEXT("USoundProcessingLibrary::GetPCMDataFromFile()"),
      TEXT("Wrong number of channels. SoundWave instance must either be mono or stereo")
    );
    return;
  }

  if (GEngine)
  {
    // Get a Pointer to the Main Audio Device
    FAudioDevice* AudioDevice = GEngine->GetMainAudioDevice();

    if (AudioDevice) {

      InSoundWave->InitAudioResource(AudioDevice->GetRuntimeFormat(InSoundWave));

      //PrintLog(TEXT("Creating new DecompressWorker."));

      // Creates a new DecompressWorker and starts it
      InitNewDecompressTask(InSoundWave);
    }
    else {
      UE_LOG(
        LogOggAsset, Error,
        TEXT("%s - %s"),
        TEXT("USoundProcessingLibrary::GetPCMDataFromFile()"),
        TEXT("Could not access the engine's main audio device")
      );
      return;
    }
  }
}

void USoundProcessingLibrary::InitNewDecompressTask(USoundWave* InSoundWaveRef)
{
  // Do we already have a valid Runnable? If not, create a new one
  if (FAudioDecompressWorker::Runnable == NULL)	{
    // Init new Worker and pass the SoundWaveRef to decompress it
    FAudioDecompressWorker::InitializeWorker(InSoundWaveRef);
  }	else if(FAudioDecompressWorker::Runnable->IsFinished()) {
    // The Worker is finished and still valid, shut it down!
    FAudioDecompressWorker::ShutdownWorker();

    // Init new Worker and pass the SoundWaveRef to decompress it
    FAudioDecompressWorker::InitializeWorker(InSoundWaveRef);
  }	else {
    UE_LOG(
      LogOggAsset, Display,
      TEXT("%s - %s"),
      TEXT("USoundProcessingLibrary::InitNewDecompressTask()"),
      TEXT("Previous decompress task was not finished yet")
    );
  }
}
