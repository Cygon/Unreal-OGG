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

#include "AudioDecompressWorker.h"

#include <AudioDevice.h>
#include <Developer/TargetPlatform/Public/Interfaces/IAudioFormat.h>

// --------------------------------------------------------------------------------------------- //

FAudioDecompressWorker* FAudioDecompressWorker::Runnable = nullptr;

int32 FAudioDecompressWorker::ThreadCounter = 0;

// --------------------------------------------------------------------------------------------- //

FAudioDecompressWorker::FAudioDecompressWorker(class USoundWave *InSoundWaveRef) :
  Thread(NULL),
	SoundWaveRef(InSoundWaveRef),
	AudioInfo(NULL) {

	if(GEngine && GEngine->GetMainAudioDevice()) {
		AudioInfo = GEngine->GetMainAudioDevice()->CreateCompressedAudioInfo(SoundWaveRef);
	}

	// Higher overall ThreadCounter to avoid duplicated names
	FAudioDecompressWorker::ThreadCounter++;

	Thread = FRunnableThread::Create(
		this,
		*FString::Printf(TEXT("FAudioDecompressWorker%d"),
		FAudioDecompressWorker::ThreadCounter),
		0,
		EThreadPriority::TPri_Normal
	);
}

// --------------------------------------------------------------------------------------------- //

FAudioDecompressWorker::~FAudioDecompressWorker() {
	delete Thread;
	Thread = NULL;
}

// --------------------------------------------------------------------------------------------- //

FAudioDecompressWorker *FAudioDecompressWorker::InitializeWorker(
	class USoundWave* InSoundWaveRef
) {
	Runnable = new FAudioDecompressWorker(InSoundWaveRef);

	return Runnable;
}

// --------------------------------------------------------------------------------------------- //

void FAudioDecompressWorker::ShutdownWorker() {
	if(Runnable) {
		Runnable->EnsureCompletion();
		delete Runnable;
		Runnable = NULL;
	}
}

// --------------------------------------------------------------------------------------------- //

bool FAudioDecompressWorker::Init() {
	// Make sure the Worker is marked is not finished
	bIsFinished = false;

	return true;
}

// --------------------------------------------------------------------------------------------- //

uint32 FAudioDecompressWorker::Run() {
	if (!SoundWaveRef) {
		return 0;
	}

	if (AudioInfo != nullptr) {
		FSoundQualityInfo QualityInfo = { 0 };

		// Parse the audio header for the relevant information
		if (!(SoundWaveRef->ResourceData))
		{
			return 0;
		}

		if (AudioInfo->ReadCompressedInfo(SoundWaveRef->ResourceData, SoundWaveRef->ResourceSize, &QualityInfo))
		{
			FScopeCycleCounterUObject WaveObject(SoundWaveRef);

			// Extract the data
			SoundWaveRef->SetSampleRate(QualityInfo.SampleRate);
			SoundWaveRef->NumChannels = QualityInfo.NumChannels;

			if (QualityInfo.Duration > 0.0f)
			{
				SoundWaveRef->Duration = QualityInfo.Duration;
			}

			// TODO: Read SampleRate property via Blueprint reflection
			int32 sampleRate = static_cast<int32>(SoundWaveRef->GetSampleRateForCurrentPlatform());

			const uint32 PCMBufferSize = SoundWaveRef->Duration * sampleRate * SoundWaveRef->NumChannels;

			SoundWaveRef->CachedRealtimeFirstBuffer = new uint8[PCMBufferSize * 2];

			AudioInfo->SeekToTime(0.0f);
			AudioInfo->ReadCompressedData(SoundWaveRef->CachedRealtimeFirstBuffer, false, PCMBufferSize * 2);
		}
		else if (SoundWaveRef->DecompressionType == DTYPE_RealTime || SoundWaveRef->DecompressionType == DTYPE_Native)
		{
			SoundWaveRef->RemoveAudioResource();
		}

		delete AudioInfo;
	}

	return 0;
}

// --------------------------------------------------------------------------------------------- //

void FAudioDecompressWorker::Stop() {
	StopTaskCounter.Increment();
}

// --------------------------------------------------------------------------------------------- //

void FAudioDecompressWorker::Exit() {
	// Make sure to mark Thread as finished
	bIsFinished = true;
}

// --------------------------------------------------------------------------------------------- //

void FAudioDecompressWorker::EnsureCompletion() {
	Stop();

	if (Thread != NULL) {

		Thread->WaitForCompletion();
	}		
}

// --------------------------------------------------------------------------------------------- //
