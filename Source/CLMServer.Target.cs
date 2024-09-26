// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

[SupportedPlatforms(UnrealPlatformClass.Server)]
public class CLMServerTarget : TargetRules
{
	public CLMServerTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Server;

		ExtraModuleNames.AddRange(new string[] { "CLMGame" });

		CLMGameTarget.ApplySharedCLMTargetSettings(this);

		bUseChecksInShipping = true;
	}
}
