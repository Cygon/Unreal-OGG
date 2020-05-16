using UnrealBuildTool;

public class SoundVorbis : ModuleRules {

  public SoundVorbis(ReadOnlyTargetRules Target) : base(Target) {
    PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

    PublicDependencyModuleNames.AddRange(
      new string[] { "Core", "CoreUObject", "Engine" }
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
