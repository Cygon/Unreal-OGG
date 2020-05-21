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

#include <SoundVorbisErrors.h>

#include <Runtime/Engine/Public/VorbisAudioInfo.h>
#include <Developer/TargetPlatform/Public/Interfaces/IAudioFormat.h>

// --------------------------------------------------------------------------------------------- //

// Barf. Holy shit Epic. I'm not sure if I should even copy & paste crap like this...
static bool bSoundVorbisFactorySuppressImportOverwriteDialog = false;

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

	// If the sound already exists, look it up so we can reuse the import settings
	USoundVorbis *existingSound = FindObject<USoundVorbis>(inParent, *name.ToString());

#if defined(THIS_COPY_PASTED_CODE_WOULD_DO_ANYTHING)
	// TODO: This needs to be sent to the audio device and wait on stopping the sounds
	TArray<UAudioComponent *> componentsToRestart;
	FAudioDeviceManager *audioDeviceManager = GEngine->GetAudioDeviceManager();
	if((audioDeviceManager != nullptr) && (existingSound != nullptr)) {
		// TODO: Generalize the stop sounds function
		audioDeviceManager->StopSoundsForReimport(ExistingSound, ComponentsToRestart);
	}
#endif

	bool bUseExistingSettings = bSoundVorbisFactorySuppressImportOverwriteDialog;

	// Bad software design 101: if your importer doesn't just import but suddenly needs
	// to take a drive with your UI library and invite the user for a talk, you'll only
	// end up with even more specialty code to make that mess testable again (oh, and
	// you've locked yourself out of 100% test coverage, too!)
	if((existingSound != nullptr) && !bUseExistingSettings && !GIsAutomationTesting)	{
		DisplayOverwriteOptionsDialog(
			FText::Format(
				NSLOCTEXT(
					"SoundVorbisImporterFactory", "ImportOverwriteWarning",
					"You are about to import '{0}' over an existing sound."
				),
				FText::FromName(name)
			)
		);

		switch(OverwriteYesOrNoToAllState) {
			case EAppReturnType::Yes:
			case EAppReturnType::YesAll: {
				bUseExistingSettings = false;
				break;
			}
			case EAppReturnType::No:
			case EAppReturnType::NoAll: {
				bUseExistingSettings = true;
				break;
			}
			default: {
				GEditor->GetEditorSubsystem<UImportSubsystem>()->BroadcastAssetPostImport(this, nullptr);
				return nullptr;
			}
		}
	}

	// Reset the flag back to false so subsequent imports are not suppressed unless
	// the code explicitly suppresses it
	bSoundVorbisFactorySuppressImportOverwriteDialog = false;

  // Make an unneccessary copy of the asset's data.
  // Seen like this in Epic's mod importer. I don't know why the buffer isn't good enough,
	// maybe they were scared the XMP library modified its input buffer?
	TArray<uint8> rawVorbisData;
	rawVorbisData.Empty(bufferEnd - buffer);
	rawVorbisData.AddUninitialized(bufferEnd - buffer);
	FMemory::Memcpy(rawVorbisData.GetData(), buffer, rawVorbisData.Num());

	// Check if this is a valid OGG Vorbis audio file
	FSoundQualityInfo soundQualityInfo;
	FVorbisAudioInfo audioInfo;
	bool wasVorbisFile = audioInfo.ReadCompressedInfo(
		rawVorbisData.GetData(), rawVorbisData.Num(), &soundQualityInfo
	);
	if(!wasVorbisFile) {
    UE_LOG(
      LogTemp, Error,
      TEXT("%s - %s"),
      TEXT("USoundVorbisImporterFactory::FactoryCreateBinary()"),
      TEXT("Imported file was not in OGG Vorbis audio format")
    );
		return nullptr;
	}

	// Use pre-existing sound if it exists and we want to keep settings,
	// otherwise create new sound and import raw data.
	USoundVorbis *sound;
	if(bUseExistingSettings && (existingSound != nullptr)) {
	  sound = existingSound;
	} else {
		sound = NewObject<USoundVorbis>(inParent, name, flags);
	}

	sound->Duration = soundQualityInfo.Duration;

	// After all the effort, just copy the OGG Vorbis file, whole, into the audio asset...
	sound->RawData.Lock(LOCK_READ_WRITE);
	void *lockedData = sound->RawData.Realloc(bufferEnd - buffer);
	FMemory::Memcpy(lockedData, buffer, bufferEnd - buffer);
	sound->RawData.Unlock();

	// Send out notifications because why shouldn't this responsibility be part
	// of the the asset importer who needs to be tightly coupled to the whole engine :-()
	GEditor->GetEditorSubsystem<UImportSubsystem>()->BroadcastAssetPostImport(this, sound);
#if defined(THIS_COPY_PASTED_CODE_WOULD_DO_ANYTHING)
	for(int32 componentIndex = 0; componentIndex < componentsToRestart.Num(); ++componentIndex) {
		componentsToRestart[componentIndex]->Play();
	}
#endif

	UE_LOG(
		LogTemp, Display,
		TEXT("%s - %s %f"),
		TEXT("USoundVorbisImporterFactory::FactoryCreateBinary()"),
		TEXT("OGG Vorbis audio file imported with length in seconds:"),
		soundQualityInfo.Duration
	);

	return sound;
}
