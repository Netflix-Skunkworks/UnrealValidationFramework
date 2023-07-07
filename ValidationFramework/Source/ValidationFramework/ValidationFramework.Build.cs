// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ValidationFramework : ModuleRules
{
	public ValidationFramework(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core", "Blutility", "OpenColorIO", "SlateCore", "ImgMedia",
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"UnrealEd",
				"Engine",
				"Slate",
				"Projects",
				"TimeManagement",
				"SlateCore", "EditorScriptingUtilities", "UMG", "EngineSettings", "UMGEditor", 
				"LevelSequence", "SettingsEditor", "SettingsEditor", "MediaPlate", "MediaAssets", "MediaUtils", 
				"ImgMedia",
				// ... add private dependencies that you statically link with here ...	
			}
			);

		if (Target.Platform == UnrealTargetPlatform.Win64)
		{
			PrivateDependencyModuleNames.Add("DisplayCluster");
			PrivateDependencyModuleNames.Add("DisplayClusterConfiguration");
			PrivateDependencyModuleNames.Add("WindowsTargetPlatform");
			PrivateDependencyModuleNames.Add("CinematicCamera");

		}
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}
