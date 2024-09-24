// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class CLMClientTarget : TargetRules
{
	public CLMClientTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Client;

		ExtraModuleNames.AddRange(new string[] { "CLMGame" });

		CLMGameTarget.ApplySharedLyraTargetSettings(this);
	}
}
