// Copyright (c) 2016 - 2018 Extra Life Studios, LLC. All rights reserved.

#include "AbilityEditor/AblAbilityEditorSettings.h"

#include "AbleEditorPrivate.h"

UAblAbilityEditorSettings::UAblAbilityEditorSettings()
	:
	m_FOV(100.0f),
	m_MuteAudio(false),
	m_AbilityStepTimeStep(EAblAbilityEditorTimeStep::ThirtyFPS),
	m_MajorLineFrequency(0.5f),
	m_MinorLineFrequency(0.25f),
	m_ShowGuidelineLabels(true),
	m_ShowDescriptiveTaskTitles(true),
	m_PreviewAsset(),
	m_AllowStaticMeshes(true),
	m_AllowSkeletalMeshes(true),
	m_AllowAnimationBlueprints(true),
	m_AllowPawnBlueprints(true),
	m_SnapTasks(false),
	m_SnapUnit(0.01f)
{

}

float UAblAbilityEditorSettings::GetAbilityTimeStepDelta() const
{
	switch (m_AbilityStepTimeStep.GetValue())
	{
		case EAblAbilityEditorTimeStep::FifteenFPS:
		{
			return 1.0f / 15.0f;
		}
		break;
		default:
		case EAblAbilityEditorTimeStep::ThirtyFPS:
		{
			return 1.0f / 30.0f;
		}
		break;
		case EAblAbilityEditorTimeStep::SixtyFPS:
		{
			return 1.0f / 60.0f;
		}
		break;
	}

	return 0.0f;
}

#if WITH_EDITOR

void UAblAbilityEditorSettings::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	SaveConfig();
}

#endif