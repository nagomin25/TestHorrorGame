// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class TestHorrorGame : ModuleRules
{
	public TestHorrorGame(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { 
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore", 
			"EnhancedInput",
			"UMG",
			"Slate",
			"SlateCore"
		});
		
		// プロジェクトのPublicフォルダをインクルードパスに追加
		PublicIncludePaths.Add(ModuleDirectory + "/Public");
	}
}
