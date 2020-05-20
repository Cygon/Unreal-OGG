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

#include "SoundVorbisWave.h"

// --------------------------------------------------------------------------------------------- //

DECLARE_CYCLE_STAT(
  TEXT("Sound Vorbis Generate Data"), STAT_SoundVorbisGeneratePCMData, STATGROUP_Audio
);

// --------------------------------------------------------------------------------------------- //

USoundVorbisWave::USoundVorbisWave(const FObjectInitializer &objectInitializer) :
  Super(objectInitializer) {

  bCanProcessAsync = true;
}

// --------------------------------------------------------------------------------------------- //

int32 USoundVorbisWave::GeneratePCMData(uint8 *pcmData, const int32 samplesNeeded) {
  SCOPE_CYCLE_COUNTER(STAT_SoundVorbisGeneratePCMData);

  int32 BytesGenerated = Super::GeneratePCMData(pcmData, samplesNeeded);
  int32 BytesDesired = (samplesNeeded * sizeof(int16)) - BytesGenerated;

  if(BytesDesired > 0) {
    // TODO: Replace with Vorbis decoder
/*    
    while (xmp_play_frame(xmpContext) == 0)
    {
            xmp_frame_info xmpFrameInfo;
            xmp_get_frame_info(xmpContext, &xmpFrameInfo);

            const int32 BytesToCopy = FMath::Min<int32>(xmpFrameInfo.buffer_size, BytesDesired);

            if (BytesToCopy > 0)
            {
                    FMemory::Memcpy((void*)(PCMData + BytesGenerated), xmpFrameInfo.buffer, BytesToCopy);

                    BytesDesired -= BytesToCopy;
                    BytesGenerated += BytesToCopy;

                    if (BytesDesired == 0)
                    {
                            if (BytesToCopy != xmpFrameInfo.buffer_size)
                            {
                                    QueueAudio((uint8*)xmpFrameInfo.buffer + BytesToCopy, xmpFrameInfo.buffer_size - BytesToCopy);
                            }
                            break;
                    }
            }
    }
*/
  }

  return BytesGenerated;
}

// --------------------------------------------------------------------------------------------- //
