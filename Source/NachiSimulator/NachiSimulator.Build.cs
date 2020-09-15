// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.
using System.IO;
using UnrealBuildTool;

public class NachiSimulator : ModuleRules
{

    private string ModulePath
    {
        get { return ModuleDirectory; }
    }

    private string zSpacePath
    {
        get { return Path.GetFullPath("D:/ZSPACE"); }
    }

    public NachiSimulator(ReadOnlyTargetRules Target) : base(Target)
	{
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "ProceduralMeshComponent" });

        PrivateDependencyModuleNames.AddRange(new string[] { });

        //if (Target.Type == TargetRules.TargetType.Server)
        //{
        //    PrivateDependencyModuleNames.AddRange(
        //    new string[]
        //    {
        //    "ProceduralMeshComponent"
        //    }
        //    );
        //}

        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
        LoadzSpace(Target);
    }


    public bool LoadzSpace(ReadOnlyTargetRules Target)
    {
        bool isLibrarySupported = false;

        if ((Target.Platform == UnrealTargetPlatform.Win64) || (Target.Platform == UnrealTargetPlatform.Win32))
        {
            isLibrarySupported = true;

            string PlatformString = (Target.Platform == UnrealTargetPlatform.Win64) ? "x64" : "x86";
            string LibrariesPath = Path.Combine(zSpacePath, "cpp/compiled/UnrealLib/");

            PublicAdditionalLibraries.Add(Path.Combine(LibrariesPath, "01_zSpace_Core" + ".lib"));
            PublicAdditionalLibraries.Add(Path.Combine(LibrariesPath, "02_zSpace_Interface" + ".lib"));
            PublicAdditionalLibraries.Add(Path.Combine(LibrariesPath, "04_zSpace_Toolsets" + ".lib"));      
            PublicAdditionalLibraries.Add(Path.Combine(LibrariesPath, "05_zSpace_ArchGeom" + ".lib"));
            PublicAdditionalLibraries.Add(Path.Combine(LibrariesPath, "06_zSpace_Housing" + ".lib"));

        }

        if (isLibrarySupported)
        {
            // Include path
            PublicIncludePaths.Add(Path.Combine(zSpacePath, "cpp"));
        }

        return isLibrarySupported;
    }
}
