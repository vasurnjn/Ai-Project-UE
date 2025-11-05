// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class DroneManagementSys : ModuleRules
{
	public DroneManagementSys(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
            "Niagara",
            "Sockets",
            "Networking"
        });

		PrivateDependencyModuleNames.AddRange(new string[] { "Niagara","SQLiteCore","SQLiteSupport" });

		PublicIncludePaths.AddRange(new string[] {
			"DroneManagementSys",
			"DroneManagementSys/Variant_Platforming",
			"DroneManagementSys/Variant_Combat",
			"DroneManagementSys/Variant_Combat/AI",
			"DroneManagementSys/Variant_SideScrolling",
			"DroneManagementSys/Variant_SideScrolling/Gameplay",
			"DroneManagementSys/Variant_SideScrolling/AI"
		});

        PublicDependencyModuleNames.Add("HTTP");
        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}
