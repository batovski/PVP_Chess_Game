// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class PVP_GameTarget : TargetRules
{
	public PVP_GameTarget( TargetInfo Target) : base(Target)
	{
        WindowsPlatform.PCHMemoryAllocationFactor = 2000;
        Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.AddRange( new string[] { "PVP_Game" } );
	}
}
