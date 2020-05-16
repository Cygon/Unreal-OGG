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
#pragma once

#include <AudioDecompress.h>

// --------------------------------------------------------------------------------------------- //

class FAudioDecompressWorker : public FRunnable {

	/// VARIABLES ///

public:

	// Singleton Instance, can access the Thread any time via static accessors 
	static FAudioDecompressWorker* Runnable;

	// Thread to run the worker FRunnable on
	FRunnableThread* Thread;

private:

	// Stop this thread? Uses Thread Safe Counter
	FThreadSafeCounter StopTaskCounter;

	// Bool to check if the thread is running
	bool bIsFinished;

	// Counter for the ThreadNames
	static int32 ThreadCounter;

protected:

	// Pointer to SoundWave Object we want to Decompress
	class USoundWave* SoundWaveRef;

	// Some Compressed Audio Information
	ICompressedAudioInfo* AudioInfo;

	/// FUNCTIONS ///

public:

	// Constructor for my De-Compressor
	FAudioDecompressWorker(class USoundWave* InSoundWaveRef);
	~FAudioDecompressWorker();

	// Custom Init function
	static FAudioDecompressWorker* InitializeWorker(class USoundWave* InSoundWaveRef);

	// Custom Shutdown function
	static void ShutdownWorker();

	// Start FRunnable Interface
	virtual bool Init();
	virtual uint32 Run();
	virtual void Stop();
	virtual void Exit();
	// End FRunnable Interface

	// Make sure Thread completed
	void EnsureCompletion();

	/// GETTER / SETTER

	class USoundWave* GetSoundWaveRef() const {
		return SoundWaveRef;
	}

	bool IsFinished() const {
		return bIsFinished;
	}
};

// --------------------------------------------------------------------------------------------- //
