// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class CLMEditorTarget : TargetRules
{
	public CLMEditorTarget( TargetInfo Target) : base(Target)
	{
        Type = TargetType.Editor;

        ExtraModuleNames.AddRange(new string[] { "CLMGame", "CLMEditor" });

        if (!bBuildAllModules)
        {
            NativePointerMemberBehaviorOverride = PointerMemberBehavior.Disallow;
        }

        CLMGameTarget.ApplySharedLyraTargetSettings(this);

        // This is used for touch screen development along with the "Unreal Remote 2" app
        EnablePlugins.Add("RemoteSession");
    }
}
