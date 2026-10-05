using UnrealBuildTool;
using System.Collections.Generic;

public class ProjectUnrealTarget : TargetRules
{
	public ProjectUnrealTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V7;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
		ExtraModuleNames.Add("ProjectUnreal");
	}
}
