#pragma region MIT License
/*
OggAsset Library
Copyright (C) 2017-2018 Scott Bishel
Extended in 2020 by Markus Ewald

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
#include <Sound/SoundWaveProcedural.h>

#include "SoundVorbisWave.generated.h"

// --------------------------------------------------------------------------------------------- //

/// Playable sound for generated audio clips that a fed from a vorbis sound asset
UCLASS()
class USoundVorbisWave : public USoundWaveProcedural {

  GENERATED_UCLASS_BODY()

  //
  // USoundVorbisWave implementation
  //

  public: UPROPERTY()
  class USoundVorbis *SoundVorbis;

  //
  // Internal implementation
  //

  public: virtual int32 GeneratePCMData(uint8 *pcmData, const int32 samplesNeeded) override;

};

// --------------------------------------------------------------------------------------------- //
