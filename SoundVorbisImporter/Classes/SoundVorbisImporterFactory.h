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
#include <Factories/Factory.h>

#include <SoundVorbis.h>

#include "SoundVorbisImporterFactory.generated.h"

// --------------------------------------------------------------------------------------------- //

/// Factory that imports Ogg Vorbis files and produces Vorbis audio assets
UCLASS(Hidecategories=Object)
class USoundVorbisImporterFactory : public UFactory {

	GENERATED_UCLASS_BODY()

	//
	// UFactory implementation
	//

  /// Creates a new SoundVorbis asset by importing it from binary data in a buffer
	/// @remarks
	///   Just like the bloaty factory class, this method drowns in arguments and
	///   they're not even documented, so I have no idea what half of them are...
  public: virtual UObject *FactoryCreateBinary(
		UClass *class_,
		UObject *inParent,
		FName name,
		EObjectFlags flags,
		UObject *context,
		const TCHAR *type,
		const uint8 *&buffer,
		const uint8 *bufferEnd,
		FFeedbackContext *warn
	) override;

};

// --------------------------------------------------------------------------------------------- //
