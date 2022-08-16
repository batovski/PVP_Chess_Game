// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class PVP_GameEditorTarget : TargetRules
{
	public PVP_GameEditorTarget( TargetInfo Target) : base(Target)
	{
        WindowsPlatform.PCHMemoryAllocationFactor = 2000;
        Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.AddRange( new string[] { "PVP_Game" } );
	}
}
