// Copyright (c) 2016 - 2018 Extra Life Studios, LLC. All rights reserved.

#include "ablAbility.h"
#include "Animation/AnimSequence.h"
#include "Animation/AnimMontage.h"
#include "Misc/Crc.h"
#include "Tasks/ablPlayAnimationTask.h"

#include "Tasks/IAblAbilityTask.h"

UAblAbility::UAblAbility(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer),
	m_Length(1.0f),
	m_Cooldown(0.0f),
	m_CacheCooldown(true),
	m_FinishAllTasks(false),
	m_PlayRate(1.0f),
	m_IsPassive(false),
	m_MaxStacks(1),
	m_RefreshDurationOnNewStack(true),
	m_AlwaysRefreshDuration(false),
	m_OnlyRefreshLoopTime(false),
	m_ResetLoopCountOnRefresh(false),
	m_Loop(false),
	m_LoopStart(0.0f),
	m_LoopEnd(1.0f),
	m_LoopMaxIterations(0U),
	m_RequiresTarget(false),
	m_Targeting(nullptr),
	m_IsChanneled(false),
	m_ChannelConditions(),
	m_MustPassAllChannelConditions(true),
	m_FailedChannelResult(EAblAbilityTaskResult::Interrupted),
	m_Tasks(),
	m_AbilityNameHash(0U),
	m_AbilityRealm(0)
{
#if WITH_EDITORONLY_DATA
	ThumbnailImage = nullptr;
	ThumbnailInfo = nullptr;
#endif
}

void UAblAbility::PostInitProperties()
{
	Super::PostInitProperties();
}

void UAblAbility::PostLoad()
{
	Super::PostLoad();

	BuildDependencyList();

#if WITH_EDITORONLY_DATA
	for (UAblAbilityTask* ReferencedTask : m_Tasks)
	{
		// Listen for any property changes on our Tasks. 
		ReferencedTask->GetOnTaskPropertyModified().AddUObject(this, &UAblAbility::OnReferencedTaskPropertyModified);
	}
#endif

	// Generate our Name hash.
	m_AbilityNameHash = FCrc::StrCrc32(*GetName());
}

EAblAbilityStartResult UAblAbility::CanAbilityExecute(UAblAbilityContext& Context) const
{
	// Check Targeting...
	if (m_Targeting != nullptr)
	{
		// If this is Async, it's safe to call it multiple times as it will poll for the results.
		m_Targeting->FindTargets(Context);

		if (m_Targeting->IsUsingAsync() && Context.HasValidAsyncHandle())
		{
			return EAblAbilityStartResult::AsyncProcessing;
		}

		if (RequiresTarget() && !Context.HasAnyTargets())
		{
			return EAblAbilityStartResult::InvalidTarget;
		}
	}

	// Check Custom BP logic
	if (!CustomCanAbilityExecuteBP(&Context))
	{
		return EAblAbilityStartResult::FailedCustomCheck;
	}

	return EAblAbilityStartResult::Success;
}

FString UAblAbility::GetDisplayName() const
{
	FString Result;
	GetName(Result);
	int32 UnderscoreIndex;

	// Chop off the variant (_A/B/C/D) from the Left side.
	if (Result.FindLastChar('_', UnderscoreIndex))
	{
		int32 UnderscoreSize = Result.Len() - UnderscoreIndex;
	
		static FString DefaultString(TEXT("Default__"));
		if (Result.Find(DefaultString) >= 0)
		{
			int32 StringSize = Result.Len() - UnderscoreSize - DefaultString.Len();
			Result = Result.Mid(DefaultString.Len(), StringSize);
		}
		else
		{
			Result = Result.LeftChop(UnderscoreSize);
		}
	}

	return Result;
}

UAnimationAsset* UAblAbility::OnGetAnimationAssetBP_Implementation(const UAblAbilityContext* Context, const FName& EventName) const
{
	return OnGetAnimationAsset(Context, EventName);
}

bool UAblAbility::ShouldCancelAbilityBP_Implementation(const UAblAbilityContext* Context, const UAblAbility* Ability, const FName& EventName) const
{
	return ShouldCancelAbility(Context, Ability, EventName);
}

float UAblAbility::CalculateCooldownBP_Implementation(const UAblAbilityContext* Context) const
{
	return CalculateCooldown(Context);
}

bool UAblAbility::CanInterruptAbilityBP_Implementation(const UAblAbilityContext* Context, const UAblAbility* CurrentAbility) const
{
	return CanInterruptAbility(Context, CurrentAbility);
}

bool UAblAbility::CustomFilterConditionBP_Implementation(const UAblAbilityContext* Context, const FName& EventName, AActor* Actor) const
{
	return CustomFilterCondition(Context, EventName, Actor);
}

int32 UAblAbility::GetMaxStacksBP_Implementation(const UAblAbilityContext* Context) const
{
	return GetMaxStacks(Context);
}

bool UAblAbility::CustomCanAbilityExecuteBP_Implementation(const UAblAbilityContext* Context) const
{
	return CustomCanAbilityExecute(Context);
}

bool UAblAbility::CustomCanLoopExecuteBP_Implementation(const UAblAbilityContext* Context) const
{
	return CustomCanLoopExecute(Context);
}

bool UAblAbility::CheckCustomChannelConditionalBP_Implementation(const UAblAbilityContext* Context, const FName& EventName) const
{
	return CheckCustomChannelConditional(Context, EventName);
}

float UAblAbility::CalculateDamageForActorBP_Implementation(const UAblAbilityContext* Context, const FName& EventName, float BaseDamage, AActor* Actor) const
{
	return CalculateDamageForActor(Context, EventName, BaseDamage, Actor);
}

bool UAblAbility::CustomCanBranchToBP_Implementation(const UAblAbilityContext* Context, const UAblAbility* BranchAbility) const
{
	return CustomCanBranchTo(Context, BranchAbility);
}

UAblAbility* UAblAbility::OnGetBranchAbilityBP_Implementation(const UAblAbilityContext* Context, const FName& EventName) const
{
	return OnGetBranchAbility(Context, EventName);
}

float UAblAbility::GetPlayRateBP_Implementation(const UAblAbilityContext* Context) const
{
	return GetPlayRate(Context);
}

void UAblAbility::OnAbilityStartBP_Implementation(const UAblAbilityContext* Context) const
{
	OnAbilityStart(Context);
}

void UAblAbility::OnAbilityEndBP_Implementation(const UAblAbilityContext* Context) const
{
	OnAbilityEnd(Context);
}

void UAblAbility::OnAbilityInterruptBP_Implementation(const UAblAbilityContext* Context) const
{
	OnAbilityInterrupt(Context);
}

void UAblAbility::OnAbilityBranchBP_Implementation(const UAblAbilityContext* Context) const
{
	OnAbilityBranch(Context);
}

void UAblAbility::OnCollisionEventBP_Implementation(const UAblAbilityContext* Context, const FName& EventName, const TArray<FAblQueryResult>& HitEntities) const
{
	OnCollisionEvent(Context, EventName, HitEntities);
}

void UAblAbility::OnRaycastEventBP_Implementation(const UAblAbilityContext* Context, const FName& EventName, const TArray<FHitResult>& HitResults) const
{
	OnRaycastEvent(Context, EventName, HitResults);
}

void UAblAbility::OnCustomEventBP_Implementation(const UAblAbilityContext* Context, const FName& EventName) const
{
	OnCustomEvent(Context, EventName);
}

void UAblAbility::OnSpawnedActorEventBP_Implementation(const UAblAbilityContext* Context, const FName& EventName, AActor* SpawnedActor) const
{
	OnSpawnedActorEvent(Context, EventName, SpawnedActor);
}

void UAblAbility::BuildDependencyList()
{
	m_AllDependentTasks.Empty();

	for (const UAblAbilityTask* Task : m_Tasks)
	{
		if (Task->HasDependencies())
		{
			for (const UAblAbilityTask* TaskDependency : Task->GetTaskDependencies())
			{
				// Make sure our Tasks and Dependencies are in the same realms (or Client/Server so they'll always run) and that they aren't stale somehow.
				if ((TaskDependency->GetTaskRealm() == Task->GetTaskRealm() || 
					TaskDependency->GetTaskRealm() == EAblAbilityTaskRealm::ClientAndServer ||
					Task->GetTaskRealm() == EAblAbilityTaskRealm::ClientAndServer) &&
					m_Tasks.Contains(TaskDependency))
				{
					m_AllDependentTasks.AddUnique(TaskDependency);
				}
			}
		}
	}
}

#if WITH_EDITOR

void UAblAbility::AddTask(UAblAbilityTask& Task)
{
	m_Tasks.Add(&Task);
}

void UAblAbility::RemoveTask(UAblAbilityTask& Task)
{
	m_Tasks.Remove(&Task);
}

void UAblAbility::RemoveTaskAtIndex(int32 Index)
{
	m_Tasks.RemoveAt(Index);
}

void UAblAbility::SortTasks()
{
	m_Tasks.Sort([](const UAblAbilityTask& A, const UAblAbilityTask& B)
	{
		return A.GetStartTime() < B.GetStartTime();
	});
}

void UAblAbility::ValidateDependencies()
{
	for (UAblAbilityTask* Task : m_Tasks)
	{
		TArray<const UAblAbilityTask*>& Dependencies = Task->GetMutableTaskDependencies();
		for (int i = 0; i < Dependencies.Num();)
		{
			// Task no longer exists in our Ability, remove it. 
			if (!m_Tasks.Contains(Dependencies[i]))
			{
				Dependencies.RemoveAt(i, 1, false);
			}
			else
			{
				++i;
			}
		}
		Dependencies.Shrink();
	}
}

void UAblAbility::MarkTasksAsPublic()
{
	for (int32 i = 0; i < m_Tasks.Num(); ++i)
	{
		m_Tasks[i]->SetFlags(m_Tasks[i]->GetFlags() | RF_Public);
		m_Tasks[i]->MarkPackageDirty();
	}
}

void UAblAbility::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	// Loop End can never be past our length.
	m_LoopEnd = FMath::Min(m_LoopEnd, m_Length);

	// Passive has priority over Channeled, so if they set the Ability to passive - revert the channeled property.
	if (m_IsPassive)
	{
		m_IsChanneled = false;
	}

	if (PropertyChangedEvent.Property && PropertyChangedEvent.Property->GetFName() == GET_MEMBER_NAME_CHECKED(UAblAbility, m_Tasks))
	{
		// Our Tasks have changed, rebuild dependencies.
		ValidateDependencies();
		BuildDependencyList();
	}

	if (!m_AbilityNameHash)
	{
		m_AbilityNameHash = FCrc::StrCrc32(*GetName());
	}

}

void UAblAbility::OnReferencedTaskPropertyModified(UAblAbilityTask& Task, struct FPropertyChangedEvent& PropertyChangedEvent)
{
	if (PropertyChangedEvent.Property && PropertyChangedEvent.Property->GetFName() == FName(TEXT("m_Dependencies")))
	{
		// Our Task changed dependencies. Validate/Rebuild.
		ValidateDependencies();
		BuildDependencyList();
	}

	if (!m_AbilityNameHash)
	{
		m_AbilityNameHash = FCrc::StrCrc32(*GetName());
	}
}

#endif
