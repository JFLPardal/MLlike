// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class MLlikeEditorTarget : TargetRules
{
	public MLlikeEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V5;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_6;
		ExtraModuleNames.Add("MLlike");

        bEnforceIWYU = true;
		// change this to test with non unity builds and with relying on PCH
		const bool bTestInclude = false;
        bUseUnityBuild = bTestInclude ? false : true;
        bUsePCHFiles = bTestInclude ? false : true;
    }
}
