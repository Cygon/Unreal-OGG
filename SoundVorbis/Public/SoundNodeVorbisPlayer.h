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

#include <CoreMinimal.h>
#include <Sound/SoundNodeAssetReferencer.h>

#include "SoundNodeVorbisPlayer.generated.h"

// --------------------------------------------------------------------------------------------- //

class USoundVorbis;

// --------------------------------------------------------------------------------------------- //

/** 
 * Sound node that contains a reference to the mod file to be played
 */
UCLASS(HideCategories=Object, EditInlineNew, Meta=(DisplayName="Vorbis Player"))
class USoundNodeVorbisPlayer : public USoundNodeAssetReferencer {

  GENERATED_BODY()

  /// Retrieves the Vorbis audio asset that the vorbis player is handling
  public:	USoundVorbis *GetSoundVorbis() const { return this->SoundVorbis; }

  /// Changes the Vorbis audio asset the vorbis player will be working with
  /// @param  soundVorbis  New Vorbis audio asset the player should work with
  public: void SetSoundVorbis(USoundVorbis *soundVorbis);

  //
  // USoundNodeAssetReferencer implementation
  //

  public: virtual void LoadAsset(bool bAddToRoot = false) override;

  public: virtual void ClearAssetReferences() override;

  //
  // USoundNode implementation
  //

  // A Vorbis Player is the end of the chain and has no children
  public: virtual int32 GetMaxChildNodes() const override { return 0; }

  /// Calculates the total duration playback of the audio asset will take in seconds
  /// @remarks
  ///   If the audio asset is a looping track, will return INDEFINITELY_LOOPING_DURATION
  public: virtual float GetDuration() override;
  
  /// I have no idea what is being "parsed" here. Nor about the parameters.
  /// @remarks
  ///   These kind of hard-to-understand and extremely specific functions that
  ///   must duplicate some standard behavior while mixing in their specifics
  ///   usually are the result of a developer who failed to find a good
  ///   interface to decouple external code with.
  ///
  ///   Check Epic's 'SoundNodeModPlayer' class and clone it again if someone ever
  ///   changes the interface here.
  public: virtual void ParseNodes(
    FAudioDevice *audioDevice,
    const UPTRINT nodeWaveInstanceHash,
    FActiveSound &activeSound,
    const FSoundParseParameters &parseParams,
    TArray<FWaveInstance *> &waveInstances
  ) override;

#if WITH_EDITOR
  /// Provides a human-readable name for the playing audio track
  public: virtual FText GetTitle() const override;
#endif

  //
  // UObject implementation
  //

  /// Loads or saves the vorbis-encoded sound using the specified archive
  /// @param  archive  Container holding or receiving the saved values
  protected: virtual void Serialize(FArchive &archive) override;

#if WITH_EDITOR
  /// Called when an attribute of the player is changed in the Unreal Editor
  /// @param  propertyChangedEvent  Contains the name of the changed attribute
  protected: virtual void PostEditChangeProperty(
    FPropertyChangedEvent &propertyChangedEvent
  ) override;
#endif

  //
  // Internal implementation
  //

  /// Notification for when the vorbis asset has finished loading asynchronously
  private: void OnSoundVorbisLoaded(
    const FName &packageName,
    UPackage *package,
    EAsyncLoadingResult::Type result,
    bool bAddToRoot
  );

  /// Asset that this player will be playing back. Can be in an unloaded state
  private: UPROPERTY(EditAnywhere, Category=VorbisPlayer, Meta=(DisplayName="Sound Vorbis"))
  TSoftObjectPtr<USoundVorbis> SoundVorbisAssetPtr;

  /// Asset behind SoundVorbisAssetPtr, once it's loaded and ready
  private: UPROPERTY(Transient)
  USoundVorbis *SoundVorbis;

  /// Whether this player will be looping whatever vorbis asset is being played
  private: UPROPERTY(EditAnywhere, Category=VorbisPlayer)
  uint32 bLooping:1;

  /// Whether the vorbis asset is currently being loaded asynchronously
  private: uint32 bAsyncLoading:1;

};

// --------------------------------------------------------------------------------------------- //
