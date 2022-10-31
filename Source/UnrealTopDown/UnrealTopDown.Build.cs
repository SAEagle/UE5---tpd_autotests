// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class UnrealTopDown : ModuleRules
{
	public UnrealTopDown(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "NavigationSystem", "AIModule", "Niagara", "Json", "JsonUtilities", "UMG", "FunctionalTesting" });

		PublicIncludePaths.Add("UnrealTopDown");
	}
}
