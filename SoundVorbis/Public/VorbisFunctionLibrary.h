#pragma region MIT License
/*
OggAsset Library
Copyright (C) 2017-2018 Scott Bishel
Enhancements copyright (C) 2020 Markus Ewald

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

#include <AudioDecompress.h>
#include <Runtime/Core/Public/Async/AsyncWork.h>

//#include "AudioDecompressWorker.h"
//#include "OggAsset.h"

#include "VorbisFunctionLibrary.generated.h"

// --------------------------------------------------------------------------------------------- //

/// Provides functions for loading ogg audio streams in Blueprints
UCLASS()
class SOUNDVORBIS_API UVorbisFunctionLibrary : public UBlueprintFunctionLibrary {

	GENERATED_BODY()

	//
	// UVorbisFunctionLibrary implementation
	//

	/// Loads a vanilla .ogg vorbis audio stream from a file on disk
	/// @param  path  Directory and filename of the file that will be loaded
	/// @returns The SoundWave instance for the loaded audio stream
	public: UFUNCTION(BlueprintCallable, Category = "Ogg")
	static USoundWave *LoadOggFile(const FString &path);

	//
	// Internal implementation
	//
/*	
	/// Loads an audio stream from the in-memory contents of an .ogg vorbis file
	/// @param  rawFile  File contents of the .ogg vorbis file
	/// @returns The SoundWave instance for the loaded audio stream
	public: static USoundWave *LoadData(const TArray<uint8> &rawFile);


  protected: static void LoadSoundWave(
		USoundWave *compressedSoundWave, const TArray<uint8> &rawFile
	);

	// Function to fill in the RawFile sound data into the USoundWave object
	protected: static bool FillSoundWaveInfo(
		class USoundWave *inSoundWave, const TArray<uint8> &inRawFile
	);

  /// Decompresses the ogg file in memory into an Unreal Engine USoundWave
	protected: static void GetPCMDataFromFile(class USoundWave *inSoundWave);

	// Function to Start a new DecompressTask
	protected: static void InitNewDecompressTask(USoundWave *inSoundWave);
*/
};

// --------------------------------------------------------------------------------------------- //
