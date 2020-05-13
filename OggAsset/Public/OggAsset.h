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

#include <Sound/SoundWave.h>

#include "OggAsset.generated.h"

/**
 * Implements an asset that can be used to store an Ogg file
 */
UCLASS(BlueprintType, HideCategories=(Object))
class OGGASSET_API UOggAsset
	: public USoundWave
{
	GENERATED_BODY()

public:

	/** Holds the stored data. */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category =Object)
	TArray<uint8> Data;

	virtual void PostLoad() override {
		USoundBase::PostLoad();

		if (Data.Num() == 0)
			return;

		// Get Pointer to the Compressed OGG Data
		FByteBulkData* BulkData = &this->CompressedFormatData.GetFormat(FName("OGG"));

		// Set the Lock of the BulkData to ReadWrite
		BulkData->Lock(LOCK_READ_WRITE);

		// Copy compressed RawFile Data to the Address of the OGG Data of the SW File
		FMemory::Memmove(BulkData->Realloc(Data.Num()), Data.GetData(), Data.Num());

		// Unlock the BulkData again
		BulkData->Unlock();
	}
};
