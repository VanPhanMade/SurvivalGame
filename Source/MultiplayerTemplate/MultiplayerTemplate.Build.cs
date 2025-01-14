// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MultiplayerTemplate : ModuleRules
{
	public MultiplayerTemplate(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { 
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore", 
			"EnhancedInput", 
			"UMG", 
			"OnlineSubsystemSteam", 
			"OnlineSubsystem", 
			"OnlineSubsystemUtils", 
			"SlateCore",
			"ProceduralMeshComponent"
			});

		PrivateDependencyModuleNames.AddRange(new string[] {  });
	}
}
