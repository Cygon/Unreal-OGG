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
#pragma once

#include "AudioDecompress.h"

// Workers, to have an Async Decompress worker
#include "Runtime/Core/Public/Async/AsyncWork.h"

#include "AudioDecompressWorker.h"

#include "OggAsset.h"

#include "SoundProcessingLibrary.generated.h"

UCLASS()
class OGGASSET_API USoundProcessingLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	/// FUNCTIONS ///

public:
	/// Functions to load Data from the HardDrive

	// Function to load a sound file from the HD
	UFUNCTION(BlueprintCallable, Category = "Ogg")
	static USoundWave* LoadOggFile(const FString& InFilePath);
	
	static USoundWave* LoadData(const TArray<uint8>& RawFile);

	static void LoadSoundWave(USoundWave* CompressedSoundWaveRef, const TArray<uint8>& RawFile);

	// Function to fill in the RawFile sound data into the USoundWave object
	static bool FillSoundWaveInfo(class USoundWave* InSoundWave, TArray<uint8>* InRawFile);

	/// Function to decompress the compressed Data that comes with the .ogg file

	static void GetPCMDataFromFile(class USoundWave* InSoundWave);

	/// Helper Functions

	// Function to Start a new DecompressTask
	static void InitNewDecompressTask(USoundWave* InSoundWaveRef);
};
