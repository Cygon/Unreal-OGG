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

#include "SoundProcessingLibrary.h"
#include "OggAssetErrors.h"
#include "OggAsset.h"

#include <Sound/SoundWave.h>
#include <AudioDevice.h>
#include <Runtime/Engine/Public/VorbisAudioInfo.h>
#include <Developer/TargetPlatform/Public/Interfaces/IAudioFormat.h>

// --------------------------------------------------------------------------------------------- //

USoundWave *USoundProcessingLibrary::LoadOggFile(const FString &path) {

  // Try to load the whole file into memory. If it can't be accessed or doesn't exist,
  // complain and return NULL (since Unreal Engine has no proper error handling)
  TArray<uint8> fileContents;
  {
    bool wasLoaded = FFileHelper::LoadFileToArray(fileContents, *path);
    if(!wasLoaded) {
      UE_LOG(
        LogOggAsset, Error,
        TEXT("%s - %s %s"),
        TEXT("USoundProcessingLibrary::LoadOggFile()"),
        TEXT("Could not load file at "),
        *path
      );
      return nullptr;
    }
  }

  // We were able to load the file, now try to decode it as Ogg Vorbis container
  return USoundProcessingLibrary::LoadData(fileContents);

}

// --------------------------------------------------------------------------------------------- //

USoundWave *USoundProcessingLibrary::LoadData(const TArray<uint8> &rawFile) {

  // Sanity check for the input data array -- if it's empty, it can't be valid
  if(rawFile.Num() == 0) {
    UE_LOG(
      LogOggAsset, Error,
      TEXT("%s - %s"),
      TEXT("USoundProcessingLibrary::LoadData()"),
      TEXT("Raw file input array is empty. Sound not loaded/assigned?")
    );
    return nullptr;
  }

  // Try to create a new USoundWave into which the 
  USoundWave *compressedSoundWave = NewObject<USoundWave>(USoundWave::StaticClass());
  if(compressedSoundWave == nullptr) {
    UE_LOG(
      LogOggAsset, Error,
      TEXT("%s - %s"),
      TEXT("USoundProcessingLibrary::LoadData()"),
      TEXT("Failed to create a new SoundWave object!")
    );
    return nullptr;
  }

  // Fill the vorbis stream into the SoundWave Object
  if(!FillSoundWaveInfo(compressedSoundWave, rawFile)) {
    UE_LOG(
      LogOggAsset, Error,
      TEXT("%s - %s"),
      TEXT("USoundProcessingLibrary::LoadData()"),
      TEXT("Could not read compressed vorbis audio stream. Asset corrupted?")
    );
    return nullptr;
  }
    
  // Get Pointer to the Compressed OGG Data
  FByteBulkData *bulkData = &compressedSoundWave->CompressedFormatData.GetFormat(FName("OGG"));

  // Set the Lock of the BulkData to ReadWrite
  bulkData->Lock(LOCK_READ_WRITE);

  // Copy compressed RawFile Data to the Address of the OGG Data of the SW File
  FMemory::Memmove(bulkData->Realloc(rawFile.Num()), rawFile.GetData(), rawFile.Num());

  // Unlock the BulkData again
  bulkData->Unlock();

  return compressedSoundWave;
}

// --------------------------------------------------------------------------------------------- //

void USoundProcessingLibrary::LoadSoundWave(
  USoundWave *compressedSoundWave, const TArray<uint8> &rawFile
) {

  // Make sure the SoundWave Object is Valid
  if(compressedSoundWave == nullptr) {
    UE_LOG(
      LogOggAsset, Error,
      TEXT("%s - %s"),
      TEXT("USoundProcessingLibrary::LoadSoundWave()"),
      TEXT("Compressed sound wave instance was null")
    );
    return;
  }

  // Fill the SoundData into the SoundWave Object
  if(rawFile.Num() == 0) {
    UE_LOG(
      LogOggAsset, Error,
      TEXT("%s - %s"),
      TEXT("USoundProcessingLibrary::LoadSoundWave()"),
      TEXT("Raw file input array is empty. Sound not loaded/assigned?")
    );
    return;
  }

  if(!FillSoundWaveInfo(compressedSoundWave, rawFile)) {
    UE_LOG(
      LogOggAsset, Error,
      TEXT("%s - %s"),
      TEXT("USoundProcessingLibrary::LoadSoundWave()"),
      TEXT("Could not read compressed vorbis audio stream. Asset corrupted?")
    );
    return;
  }

  // Get Pointer to the Compressed OGG Data
  FByteBulkData *bulkData = &compressedSoundWave->CompressedFormatData.GetFormat(FName("OGG"));

  // Set the Lock of the BulkData to ReadWrite
  bulkData->Lock(LOCK_READ_WRITE);

  // Copy compressed RawFile Data to the Address of the OGG Data of the SW File
  FMemory::Memmove(bulkData->Realloc(rawFile.Num()), rawFile.GetData(), rawFile.Num());

  // Unlock the BulkData again
  bulkData->Unlock();
}

// --------------------------------------------------------------------------------------------- //

bool USoundProcessingLibrary::FillSoundWaveInfo(
  USoundWave *inSoundWave, const TArray<uint8> &inRawFile
) {

  // Info Structs
  FSoundQualityInfo soundQualityInfo;
  FVorbisAudioInfo vorbisAudioInfo;

  // Save the Info into SoundQualityInfo
  if(!vorbisAudioInfo.ReadCompressedInfo(inRawFile.GetData(), inRawFile.Num(), &soundQualityInfo)) {
    return false;
  }

  // Fill in all the Data we have
  inSoundWave->DecompressionType = EDecompressionType::DTYPE_RealTime;
  inSoundWave->SoundGroup = ESoundGroup::SOUNDGROUP_Default;
  inSoundWave->NumChannels = soundQualityInfo.NumChannels;
  inSoundWave->Duration = soundQualityInfo.Duration;
  inSoundWave->RawPCMDataSize = soundQualityInfo.SampleDataSize;
  inSoundWave->SetSampleRate(soundQualityInfo.SampleRate);

  return true;
}

// --------------------------------------------------------------------------------------------- //

/// Function to decompress the compressed Data that comes with the .ogg file

void USoundProcessingLibrary::GetPCMDataFromFile(USoundWave *inSoundWave) {
  if(inSoundWave == nullptr)	{
    UE_LOG(
      LogOggAsset, Error,
      TEXT("%s - %s"),
      TEXT("USoundProcessingLibrary::GetPCMDataFromFile()"),
      TEXT("Passed sound wave is null")
    );
    return;
  }

  // We only support playback or mono or stereo streams
  bool isMonoOrStereo = (
    (inSoundWave->NumChannels == 1) || (inSoundWave->NumChannels == 2)
  );
  if(!isMonoOrStereo) {
    UE_LOG(
      LogOggAsset, Error,
      TEXT("%s - %s"),
      TEXT("USoundProcessingLibrary::GetPCMDataFromFile()"),
      TEXT("Wrong number of channels. SoundWave instance must either be mono or stereo")
    );
    return;
  }

  if(GEngine) {
    // Get a Pointer to the Main Audio Device
    FAudioDevice* AudioDevice = GEngine->GetMainAudioDevice();

    if (AudioDevice) {

      inSoundWave->InitAudioResource(AudioDevice->GetRuntimeFormat(inSoundWave));

      //PrintLog(TEXT("Creating new DecompressWorker."));

      // Creates a new DecompressWorker and starts it
      InitNewDecompressTask(inSoundWave);
    } else {
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

// --------------------------------------------------------------------------------------------- //

void USoundProcessingLibrary::InitNewDecompressTask(USoundWave *InSoundWaveRef) {
  // Do we already have a valid Runnable? If not, create a new one
  if(FAudioDecompressWorker::Runnable == NULL) {
    // Init new Worker and pass the SoundWaveRef to decompress it
    FAudioDecompressWorker::InitializeWorker(InSoundWaveRef);
  } else if(FAudioDecompressWorker::Runnable->IsFinished()) {
    // The Worker is finished and still valid, shut it down!
    FAudioDecompressWorker::ShutdownWorker();

    // Init new Worker and pass the SoundWaveRef to decompress it
    FAudioDecompressWorker::InitializeWorker(InSoundWaveRef);
  } else {
    UE_LOG(
      LogOggAsset, Display,
      TEXT("%s - %s"),
      TEXT("USoundProcessingLibrary::InitNewDecompressTask()"),
      TEXT("Previous decompress task was not finished yet")
    );
  }
}

// --------------------------------------------------------------------------------------------- //
