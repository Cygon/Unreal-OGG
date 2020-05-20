#pragma region MIT License
/*
OggAsset Library
Copyright (C) 2017-2018 Scott Bishel
Enhancements copyright (C) 2020 Markus Ewald

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, Vorbisify, merge, publish, distribute, sublicense, and/or sell
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

#include "SoundNodeVorbisPlayer.h"
#include "SoundVorbis.h"

#include <UObject/FrameworkObjectVersion.h>
#include <ActiveSound.h>

// --------------------------------------------------------------------------------------------- //

#define LOCTEXT_NAMESPACE "SoundNodeVorbisPlayer"

// --------------------------------------------------------------------------------------------- //

void USoundNodeVorbisPlayer::Serialize(FArchive &archive) {
  Super::Serialize(archive);

  archive.UsingCustomVersion(FFrameworkObjectVersion::GUID);

  // This is copied straight from USoundMod, so we could probably remove it
  // since there hasn't ever been a version of USoundVorbis without hard references
  int32 archiveVersion = archive.CustomVer(FFrameworkObjectVersion::GUID);
  if(archiveVersion >= FFrameworkObjectVersion::HardSoundReferences) {
    if(archive.IsLoading()) {
      archive << SoundVorbis;
    }	else if(archive.IsSaving()) {
      USoundVorbis *hardReference = (ShouldHardReferenceAsset() ? this->SoundVorbis : nullptr);
      archive << hardReference;
    }

  }
}

// --------------------------------------------------------------------------------------------- //

void USoundNodeVorbisPlayer::LoadAsset(bool bAddToRoot) {
  if(IsAsyncLoading()) {
    this->SoundVorbis = this->SoundVorbisAssetPtr.Get();
    if(this->SoundVorbis == nullptr) {
      const FString longPackageName = this->SoundVorbisAssetPtr.GetLongPackageName();
      if(!longPackageName.IsEmpty()) {
        bAsyncLoading = true;
        LoadPackageAsync(
          longPackageName,
          FLoadPackageAsyncDelegate::CreateUObject(
            this, &USoundNodeVorbisPlayer::OnSoundVorbisLoaded, bAddToRoot
          )
        );
      }
    } else if(bAddToRoot) {
      this->SoundVorbis->AddToRoot();
    }
  } else {
    this->SoundVorbis = this->SoundVorbisAssetPtr.LoadSynchronous();
    if(bAddToRoot && (this->SoundVorbis != nullptr)) {
      this->SoundVorbis->AddToRoot();
    }
  }
}

// --------------------------------------------------------------------------------------------- //

void USoundNodeVorbisPlayer::ClearAssetReferences() {
  this->SoundVorbis = nullptr;
}

// --------------------------------------------------------------------------------------------- //

void USoundNodeVorbisPlayer::OnSoundVorbisLoaded(
  const FName &packageName, UPackage *package, EAsyncLoadingResult::Type result, bool bAddToRoot
) {
  if(result == EAsyncLoadingResult::Succeeded) {
    this->SoundVorbis = this->SoundVorbisAssetPtr.Get();
    if(bAddToRoot && (this->SoundVorbis != nullptr)) {
      this->SoundVorbis->AddToRoot();
    }
  }

  bAsyncLoading = false;
}

// --------------------------------------------------------------------------------------------- //

void USoundNodeVorbisPlayer::SetSoundVorbis(USoundVorbis *inSoundVorbis) {
  this->SoundVorbis = inSoundVorbis;
  this->SoundVorbisAssetPtr = inSoundVorbis;
}

// --------------------------------------------------------------------------------------------- //

#if WITH_EDITOR
void USoundNodeVorbisPlayer::PostEditChangeProperty(FPropertyChangedEvent &propertyChangedEvent) {

  // This just builds the string "SoundVorbisAssetPtr" but should result in
  // a compile time error if said attribute doesn't exist
  FName soundVorbisAssetPtrName = (
    GET_MEMBER_NAME_CHECKED(USoundNodeVorbisPlayer, SoundVorbisAssetPtr)
  );

  // Does the changed property affect which asset this player is dealing with?
  // If so, load the new asset that has been selected
  bool affectsAsset = (
    (propertyChangedEvent.Property != nullptr) &&
    (propertyChangedEvent.Property->GetFName() == soundVorbisAssetPtrName)
  );
  if(affectsAsset) {
    LoadAsset();
  }
}
#endif // WITH_EDITOR

// --------------------------------------------------------------------------------------------- //

void USoundNodeVorbisPlayer::ParseNodes(
  FAudioDevice *audioDevice,
  const UPTRINT nodeWaveInstanceHash,
  FActiveSound &activeSound,
  const FSoundParseParameters &parseParams,
  TArray<FWaveInstance *> &waveInstances
) {
  if(this->bAsyncLoading) {
    this->SoundVorbis = this->SoundVorbisAssetPtr.LoadSynchronous();
    this->bAsyncLoading = false;
  }

  if(this->SoundVorbis != nullptr) {

    // The SoundWave's bLooping is only for if it is directly referenced,
    // so clear it in the case that it is being played from a player
    bool wasAssetLooping = this->SoundVorbis->bLooping;
    this->SoundVorbis->bLooping = false;
    {
      // TODO: This could use a ScopeGuard. On the other hand, UE only uses accidental
      // exceptions and all error handling is done... well, it's not done at all.
      if(this->bLooping) {
        FSoundParseParameters updatedParams = parseParams;
        updatedParams.bLooping = true;
        this->SoundVorbis->Parse(
          audioDevice, nodeWaveInstanceHash, activeSound, updatedParams, waveInstances
        );
      } else {
        this->SoundVorbis->Parse(
          audioDevice, nodeWaveInstanceHash, activeSound, parseParams, waveInstances
        );
      }
    }
    this->SoundVorbis->bLooping = wasAssetLooping;
  }
}

// --------------------------------------------------------------------------------------------- //

float USoundNodeVorbisPlayer::GetDuration() {
  float duration = 0.f;
  
  if(SoundVorbis) {
    if(bLooping) {
      duration = INDEFINITELY_LOOPING_DURATION;
    } else {
      duration = this->SoundVorbis->Duration;
    }
  }
  
  return duration;
}

// --------------------------------------------------------------------------------------------- //

#if WITH_EDITOR
FText USoundNodeVorbisPlayer::GetTitle() const {

  // Fetch the name of the audio asset handled by this player, if any
  FText soundVorbisName;
  if(this->SoundVorbis) {
    soundVorbisName = FText::FromString(this->SoundVorbis->GetFName().ToString());
  } else {
    soundVorbisName = LOCTEXT("NoSoundVorbis", "NONE");
  }

  // Build the name including an indicating whether the sound is looping
  FText title;
  {
    FFormatNamedArguments arguments;
    arguments.Add(TEXT("Description"), Super::GetTitle());
    arguments.Add(TEXT("SoundVorbisName"), soundVorbisName);

    if(bLooping) {
      title = FText::Format(
        LOCTEXT("LoopingSoundVorbisDescription", "Looping {Description} : {SoundVorbisName}"),
        arguments
      );
    } else {
      title = FText::Format(
        LOCTEXT("NonLoopingSoundVorbisDescription", "{Description} : {SoundVorbisName}"),
        arguments
      );
    }
  }

  return title;
}
#endif // WITH_EDITOR

// --------------------------------------------------------------------------------------------- //

#undef LOCTEXT_NAMESPACE

// --------------------------------------------------------------------------------------------- //
