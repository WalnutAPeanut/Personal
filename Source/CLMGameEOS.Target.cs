// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class CLMGameEOSTarget : CLMGameTarget
{
	public CLMGameEOSTarget(TargetInfo Target) : base(Target)
	{
		CustomConfig = "EOS";
	}
}
