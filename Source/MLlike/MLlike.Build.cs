// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MLlike : ModuleRules
{
	public MLlike(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"NavigationSystem",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"Niagara",
			"UMG",
			"Slate",
            "GameplayAbilities",
			"GameplayTags",
			"GameplayTasks",
            "GameplayMessageRuntime"
        });

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"MLlike",
			"MLlike/Variant_Strategy",
			"MLlike/Variant_Strategy/UI",
			"MLlike/Variant_TwinStick",
			"MLlike/Variant_TwinStick/AI",
			"MLlike/Variant_TwinStick/Gameplay",
			"MLlike/Variant_TwinStick/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
