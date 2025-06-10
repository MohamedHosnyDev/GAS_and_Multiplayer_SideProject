// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class GAS_and_Multiplayer : ModuleRules
{
	public GAS_and_Multiplayer(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" , "GameplayAbilities" , "GameplayTasks" , "GameplayTags" ,"UMG"});
	}
}
