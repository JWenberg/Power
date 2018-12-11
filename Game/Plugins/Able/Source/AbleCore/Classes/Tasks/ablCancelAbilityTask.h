// Copyright (c) 2016 - 2018 Extra Life Studios, LLC. All rights reserved.

#pragma once

#include "Tasks/IAblAbilityTask.h"
#include "GameplayTagContainer.h"
#include "UObject/ObjectMacros.h"

#include "ablCancelAbilityTask.generated.h"

#define LOCTEXT_NAMESPACE "AblAbilityTask"

UENUM()
enum EAblCancelAbilityPassiveBehavior
{
	RemoveOneStack UMETA(DisplayName = "Remove One"),
	RemoveOneStackWithRefresh UMETA(DisplayName = "Remove One and Refresh Duration"),
	RemoveEntireStack UMETA(DisplayName = "Clear Stack"),
};

UCLASS(EditInlineNew)
class UAblCancelAbilityTask : public UAblAbilityTask
{
	GENERATED_BODY()
public:
	UAblCancelAbilityTask(const FObjectInitializer& ObjectInitializer);
	virtual ~UAblCancelAbilityTask();

	/* Start our Task. */
	virtual void OnTaskStart(const TWeakObjectPtr<const UAblAbilityContext>& Context) const override;

	/* Returns true if our Task only lasts for a single frame. */
	virtual bool IsSingleFrame() const override { return true; }

	/* Returns true if our Task is Async. */
	virtual bool IsAsyncFriendly() const override { return false; }

	/* Returns the realm our Task belongs to. */
	virtual EAblAbilityTaskRealm GetTaskRealm() const override { return EAblAbilityTaskRealm::Server; }

	/* Returns the Profiler Stat ID for our Task. */
	virtual TStatId GetStatId() const override;

#if WITH_EDITOR
	/* Returns the category for our Task. */
	virtual FText GetTaskCategory() const override { return LOCTEXT("AblCancelAbilityCategory", "Gameplay"); }

	/* Returns the name of our Task. */
	virtual FText GetTaskName() const override { return LOCTEXT("AblCancelAbilityTask", "Cancel Ability"); }

	/* Returns the dynamic, descriptive name of our Task. */
	virtual FText GetDescriptiveTaskName() const override;

	/* Returns the description of our Task. */
	virtual FText GetTaskDescription() const override { return LOCTEXT("AblCancelAbilityTaskDesc", "Cancels Abilities on the Targets that match the provided critera."); }

	/* Returns the color of our Task. */
	virtual FLinearColor GetTaskColor() const override { return FLinearColor(0.93f, 0.25f, 0.95f); } // Plum-ish

	/* Returns how to display the End time of our Task. */
	virtual EVisibility ShowEndTime() const { return EVisibility::Collapsed; }
#endif
protected:
	// Simple helper method.
	bool ShouldCancelAbility(const UAblAbility& Ability, const UAblAbilityContext& Context) const;

	/* The Ability to Play. */
	UPROPERTY(EditAnywhere, Category = "Ability", meta = (DisplayName = "Ability"))
	TSubclassOf<UAblAbility> m_Ability;

	/* A Tag query to run on targets. All Abilities that match the query will be cancelled. */
	UPROPERTY(EditAnywhere, Category = "Ability|Tags", meta = (DisplayName = "Tag Query"))
	FGameplayTagQuery m_TagQuery;

	/* How to handle Passives Abilities that match the cancel criteria.*/
	UPROPERTY(EditAnywhere, Category = "Ability", meta = (DisplayName = "Passive Behavior"))
	TEnumAsByte<EAblCancelAbilityPassiveBehavior> m_PassiveBehavior;

	/* What result to pass Abilities that are canceled */
	UPROPERTY(EditAnywhere, Category = "Ability", meta = (DisplayName = "Cancel Result"))
	TEnumAsByte<EAblAbilityTaskResult> m_CancelResult;

	/* A String identifier you can use to identify this specific task in the ability blueprint, when ShouldCancelAbility is called. */
	UPROPERTY(EditAnywhere, Category = "Ability|Dynamic", meta = (DisplayName = "Event Name"))
	FName m_EventName;
};

#undef LOCTEXT_NAMESPACE
