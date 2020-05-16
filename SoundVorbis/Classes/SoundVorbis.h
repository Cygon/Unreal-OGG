#pragma region MIT License
/*
OggAsset Library
Copyright (C) 2017-2018 Scott Bishel
With enhancements copyright (C) 2020 Markus Ewald

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
#include <Sound/SoundBase.h>
#include <Serialization/BulkData.h>

#include "SoundVorbis.generated.h"

// --------------------------------------------------------------------------------------------- //

/// Streamable audio data encoded with the Vorbis codec for on-the-fly decoding
/// @remarks
///   This is the asset type that is produced by the imported and which gets stored in
///   your final, cooked build.
UCLASS(BlueprintType, HideCategories=(Object))
class OGGASSET_API USoundVorbis : public USoundBase {

  GENERATED_BODY()

  /// Intializes a new vorbis audio asset
  /// @param  objectInitializer  Container for the values to initialized the instance with
  public: USoundVorbis(const FObjectInitializer &objectInitializer);

  //
  // USoundVorbis implementation
  //

  /// Whether the sound will be played in a loop if played directly
  public:	UPROPERTY(EditAnywhere, Category=SoundVorbis)
  uint32 bLooping:1;

  //
  // USoundBase implementation
  //

  /// Whether the sound is valid and ready to be played back
  public: virtual bool IsPlayable() const override;

  /// I have no idea what is being "parsed" here. Nor about the parameters.
  /// @remarks
  ///   These kind of hard-to-understand and extremely specific functions that
  ///   must duplicate some standard behavior while mixing in their specifics
  ///   usually are the result of a developer who failed to find a good
  ///   interface to decouple external code with.
  ///
  ///   Check Epic's 'SoundMod' class and clone it again if someone ever
  ///   changes the interface here.
  public: virtual void Parse(
    class FAudioDevice *audioDevice,
    const UPTRINT nodeWaveInstanceHash,
    FActiveSound &activeSound,
    const FSoundParseParameters &parseParams,
    TArray<FWaveInstance *> &waveInstances
  ) override;

  //
  // UObject implementation
  //

  /// Loads or saves the vorbis-encoded sound using the specified archive
  /// @param  archive  Container holding or receiving the saved values
  public: virtual void Serialize(FArchive &archive) override;

  //
  // Internal implementation
  //

  /// The vorbis-encoded audio stream
  public: FByteBulkData RawData;

  /// Audio streaming buffer of the paired SoundVorbisWave instance
  private: uint8 *resourceData;

};

// --------------------------------------------------------------------------------------------- //
