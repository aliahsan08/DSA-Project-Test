// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class dsaproject : ModuleRules
{
	public dsaproject(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		// 1. PUBLIC DEPENDENCIES (Core Engine features)
		// Added "UMG" for Widgets and "Paper2D" for Sprites
		PublicDependencyModuleNames.AddRange(new string[] { 
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore", 
			"Paper2D",
			"UMG" 
		});

		// 2. PRIVATE DEPENDENCIES (UI and Input systems)
		// Added "Slate" and "SlateCore" to fix the FInputChord Linker Error
		PrivateDependencyModuleNames.AddRange(new string[] { 
			"Slate", 
			"SlateCore" 
		});
	}
}