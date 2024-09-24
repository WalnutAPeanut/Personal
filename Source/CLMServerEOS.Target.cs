// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class CLMServerEOSTarget : CLMServerTarget
{
	public CLMServerEOSTarget(TargetInfo Target) : base(Target)
	{
		CustomConfig = "EOS";
	}
}
