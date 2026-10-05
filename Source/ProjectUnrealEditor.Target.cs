using UnrealBuildTool;
using System.Collections.Generic;

public class ProjectUnrealEditorTarget : TargetRules
{
	public ProjectUnrealEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V7;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
		ExtraModuleNames.Add("ProjectUnreal");
	}
}
