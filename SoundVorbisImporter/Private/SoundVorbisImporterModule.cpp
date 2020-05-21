// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.


#include <Modules/ModuleManager.h>
#include <Modules/ModuleInterface.h>

#define LOCTEXT_NAMESPACE "FSoundVorbisImporterModule"


/**
 * Implements the TextAssetEditor module.
 */
class FSoundVorbisImporterModule : public IModuleInterface {
public:

	// IModuleInterface interface

	virtual void StartupModule() override { }
	virtual void ShutdownModule() override { }

	virtual bool SupportsDynamicReloading() override
	{
		return true;
	}
};


IMPLEMENT_MODULE(FSoundVorbisImporterModule, SoundVorbisImporter);


#undef LOCTEXT_NAMESPACE
