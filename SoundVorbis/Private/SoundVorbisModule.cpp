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

#include <Modules/ModuleManager.h>
#include <Modules/ModuleInterface.h>

// --------------------------------------------------------------------------------------------- //

/// Manages global data of the SoundVorbis module when loaded into Unreal Engine
class FSoundVorbisModule : public IModuleInterface {

  //
  // IModuleInterface implementation
  //

  /// Called when the module is loaded into Unreal Engine
  public: virtual void StartupModule() override {}

  /// Called before the module is unloaded by Unreal Engine
  public: virtual void ShutdownModule() override {}

  /// Reports the the module can be unloaded and reloaded without quitting the engine
  public: virtual bool SupportsDynamicReloading() override {
    return true;
  }

};

// --------------------------------------------------------------------------------------------- //

IMPLEMENT_MODULE(FSoundVorbisModule, SoundVorbis);

// --------------------------------------------------------------------------------------------- //
