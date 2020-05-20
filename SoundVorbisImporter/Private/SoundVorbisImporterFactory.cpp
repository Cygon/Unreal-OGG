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

#include "SoundVorbisImporterFactory.h"

//#include "../OggAsset/Public/SoundProcessingLibrary.h"

// --------------------------------------------------------------------------------------------- //

USoundVorbisImporterFactory::USoundVorbisImporterFactory(
	const FObjectInitializer &objectInitializer
) :
	Super(objectInitializer) {

	bCreateNew = false;
	SupportedClass = USoundVorbis::StaticClass();

	bEditorImport = true;

	Formats.Add(
		FString(TEXT("ogg;")) +
		NSLOCTEXT("USoundVorbisImporterFactory", "Sound", "OGG Vorbis Audio").ToString()
	);
}

// --------------------------------------------------------------------------------------------- //

UObject *USoundVorbisImporterFactory::FactoryCreateBinary(
	UClass *class_,
	UObject *inParent,
	FName name,
	EObjectFlags flags,
	UObject *context,
	const TCHAR *type,
	const uint8 *&buffer,
	const uint8 *bufferEnd,
	FFeedbackContext *warn
) {
	return nullptr;
	/*
	// If the sound already exists, look it up so we can reuse the import settings
	USoundVorbis *existingSound = FindObject<USoundVorbis>(inParent, *name.ToString());

	UOggAsset *oggAsset = nullptr;
	TArray<uint8> data;

	if (FFileHelper::LoadFileToArray(data, *CurrentFilename))
	{
		OggAsset = NewObject<UOggAsset>(InParent, Name, Flags);
		USoundProcessingLibrary::LoadSoundWave(OggAsset, data);
		OggAsset->Data = data;
	}

	return OggAsset;
	*/
}
