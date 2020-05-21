using UnrealBuildTool;

public class SoundVorbisImporter : ModuleRules {

  public SoundVorbisImporter(ReadOnlyTargetRules Target) : base(Target) {
    PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

    DynamicallyLoadedModuleNames.AddRange(
      new string[] {
        "AssetTools",
        "MainFrame",
      }
    );
/*
    PrivateIncludePaths.AddRange(
      new string[] {
        "OggAssetEditor/Private",
        "OggAssetEditor/Private/Factories",
      }
    );
*/
    PrivateDependencyModuleNames.AddRange(
      new string[] {
        "ContentBrowser",
        "Core",
        "CoreUObject",
        "DesktopWidgets",
        "EditorStyle",
        "Engine",
        "InputCore",
        "Projects",
        "Slate",
        "SlateCore",
        "SoundVorbis",
        "UnrealEd",
      }
    );

    PrivateIncludePathModuleNames.AddRange(
      new string[] {
        "AssetTools",
        "UnrealEd",
      }
    );

    PublicDefinitions.Add("WITH_OGGVORBIS");

    // Uncomment if you are using Slate UI
    // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

    // Uncomment if you are using online features
    // PrivateDependencyModuleNames.Add("OnlineSubsystem");

    // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file
    // with the Enabled attribute set to true
  }

}
