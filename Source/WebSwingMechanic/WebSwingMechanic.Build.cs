// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using UnrealBuildTool.Rules;

public class WebSwingMechanic : ModuleRules
{
	public WebSwingMechanic(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "CableComponent", "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
	}
}
