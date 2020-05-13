Unreal-OGG
==========

This is a C++ module allowing you to import .ogg Vorbis audio files
(https://xiph.org/vorbis/) into Unreal Engine without transcoding.

Unreal Engine already uses the Vorbis codec for streamed audio,
so this does not result in any new dependencies.

It is also able to load vanilla .ogg files at runtime, enabling your
game to play custom music from the end-user's hard drive.


History
-------

This code was originally an Unreal Engine plugin written
by Scott Bishel aka Geromatic in 2017/2018

https://github.com/Geromatic/Unreal-OGG

Updated and modified by Markus Ewald aka Cygon in 2020

https://github.com/Cygon/Unreal-OGG


How to Use
----------

This version of Unreal-OGG is used as a code module in your
Unreal Engine C++ project.

1. Obviously, you need an Unreal Engine C++ project first

2. Place the `OggAsset` directory in your project's `Source`
   directory (next to your game's own C++ module directory)

    MyAwesomeGame/
        Source/
            MyAwesomeGame/
            OggAsset/

3. Add the module to your game's `.uproject` file, like this:

    {
            "FileVersion": 3,
            "EngineAssociation": "4.24",
            "Category": "",
            "Description": "",
            "Modules": [
                    {
                            "Name": "MyAwesomeGame",
                            "Type": "Runtime",
                            "LoadingPhase": "Default",
                            "AdditionalDependencies": [
                                    "Engine",
                                    "CoreUObject",
                                    "LevelSequence"
                            ]
                    },
                    {
                            "Name": "OggAsset",
                            "Type": "Runtime",
                            "LoadingPhase": "Default",
                            "AdditionalDependencies": [
                                    "Engine",
                                    "CoreUObject"
                            ]
                    }
            ],
    ...

4. Add the module to your build rules in `Source/MyAwesomeGame.Target.cs`

    using UnrealBuildTool;
    using System.Collections.Generic;
    
    public class MyAwesomeGameTarget : TargetRules {
    
            public MyAwesomeGameTarget(TargetInfo Target) : base(Target) {
                    Type = TargetType.Game;
                    DefaultBuildSettings = BuildSettingsVersion.V2;
                    ExtraModuleNames.AddRange( new string[] { "MyAwesomeGame", "OggAsset" } );
            }
    
    }

5. *This isn't complete yet, I'm still in the process of converting/updating the module*
