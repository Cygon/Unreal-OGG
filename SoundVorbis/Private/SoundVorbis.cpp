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

#include "SoundVorbis.h"
#include "SoundVorbisWave.h"

#include <ActiveSound.h>

// --------------------------------------------------------------------------------------------- //

USoundVorbis::USoundVorbis(const FObjectInitializer &objectInitializer) :
  Super(objectInitializer) {}

// --------------------------------------------------------------------------------------------- //

void USoundVorbis::Serialize(FArchive &archive) {
  Super::Serialize(archive);

  this->RawData.Serialize(archive, this);
}

// --------------------------------------------------------------------------------------------- //

void USoundVorbis::Parse(
  class FAudioDevice *audioDevice,
  const UPTRINT nodeWaveInstanceHash,
  FActiveSound &activeSound,
  const FSoundParseParameters &parseParams,
  TArray<FWaveInstance *> &waveInstances
) {
  FWaveInstance *waveInstance = activeSound.FindWaveInstance(nodeWaveInstanceHash);

  // Create a new WaveInstance if this SoundWave doesn't already have one associated with it.
  if(waveInstance == nullptr) {
    const int32 SampleRate = 44100;

    // Create a new wave instance and associate with the ActiveSound
    waveInstance = &activeSound.AddWaveInstance(nodeWaveInstanceHash);

    // Create streaming wave object
    USoundVorbisWave *vorbisWave = NewObject<USoundVorbisWave>();
    vorbisWave->SetSampleRate(SampleRate);
    vorbisWave->NumChannels = 2; // TODO: Take from Vorbis file!
    vorbisWave->Duration = INDEFINITELY_LOOPING_DURATION;
    vorbisWave->bLooping = bLooping;

    // Fill the initial sound buffer (what is this? is there no way for an Unreal Engine
    // asset to get a /resource stream/ and it's always, always fully in memory? :-/)
    if(this->resourceData == nullptr) {
      this->RawData.GetCopy((void**)&this->resourceData, true);
    }

    //vorbisWave->xmpContext = xmp_create_context();
    //xmp_load_module_from_memory(ModWave->xmpContext, ResourceData, RawData.GetBulkDataSize());
    //xmp_start_player(ModWave->xmpContext, SampleRate, 0);

    waveInstance->WaveData = vorbisWave;
  }

  waveInstance->WaveData->Parse(
    audioDevice, nodeWaveInstanceHash, activeSound, parseParams, waveInstances
  );
}

// --------------------------------------------------------------------------------------------- //

bool USoundVorbis::IsPlayable() const {
  return true;
}

// --------------------------------------------------------------------------------------------- //
