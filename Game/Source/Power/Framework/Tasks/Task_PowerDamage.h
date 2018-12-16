// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tasks/ablCustomTask.h"
#include "UObject/ObjectMacros.h"
#include "Task_PowerDamage.generated.h"

#define LOCTEXT_NAMESPACE "AblAbilityTask"
/**
 * 
 */
UCLASS(EditInlineNew, hidecategories = ("Targets"))
class POWER_API UTask_PowerDamage : public UAblCustomTask
{
	GENERATED_BODY()
public:
	UTask_PowerDamage(const FObjectInitializer& ObjectInitializer);
	virtual ~UTask_PowerDamage();

	/* Start our Task. */
	virtual void OnTaskStart(const TWeakObjectPtr<const UAblAbilityContext>& Context) const override;

	/* End our Task. */
	virtual void OnTaskEnd(const TWeakObjectPtr<const UAblAbilityContext>& Context, const EAblAbilityTaskResult result) const override;

	/* Returns true if our Task only lasts for a single frame. */
	virtual bool IsSingleFrame() const override { return true; }

	/* Returns true if our Task is Async. */
	virtual bool IsAsyncFriendly() const override { return false; }

	/* Returns the realm our Task belongs to. */
	virtual EAblAbilityTaskRealm GetTaskRealm() const override { return m_TaskRealm; }

#if WITH_EDITOR
	/* Returns the category of our Task. */
	virtual FText GetTaskCategory() const override { return LOCTEXT("AblDamageEventCategory", "Damage"); }

	/* Returns the name of our Task. */
	virtual FText GetTaskName() const override { return LOCTEXT("AblDamageEventTask", "Power: Apply Damage"); }

	/* Returns the description of our Task. */
	virtual FText GetTaskDescription() const override { return LOCTEXT("AblDamageEventTaskDesc", "Calls the OnDamageEvent Blueprint Event on the owning Ability."); }

	/* Returns the color of our Task. */
	virtual FLinearColor GetTaskColor() const override { return FLinearColor(1.0f, 1.0f, 1.0f); } //Pinkish

	/* Returns estimated runtime cost for our Task. */
	virtual float GetEstimatedTaskCost() const override { return UAblAbilityTask::GetEstimatedTaskCost() + ABLTASK_EST_BP_EVENT; }

	/* Return how to handle displaying the End time of our Task. */
	virtual EVisibility ShowEndTime() const { return EVisibility::Collapsed; }

	/* Returns true if the user is allowed to edit this Tasks realm.*/
	virtual bool CanEditTaskRealm() const override { return true; }
#endif
protected:
	/* Helper method to grab all Actors who will be damaged. */
	void GetDamageTargets(const TWeakObjectPtr<const UAblAbilityContext>& Context, TArray<TWeakObjectPtr<AActor>>& OutArray) const;

	/* The damage amount. */
	UPROPERTY(EditAnywhere, Category = "Damage", meta = (DisplayName = "Damage Amount"))
		float m_Damage;

	/* Who to set as the "Source" of this damage. */
	UPROPERTY(EditAnywhere, Category = "Damage", meta = (DisplayName = "Damage Source"))
		TEnumAsByte<EAblAbilityTargetType> m_DamageSource;

	/* Who to apply the damage to. */
	UPROPERTY(EditAnywhere, Category = "Damage", meta = (DisplayName = "Damage Targets"))
		TArray<TEnumAsByte<EAblAbilityTargetType>> m_DamageTargets;

	/* A String identifier you can use to identify this specific task in the ability blueprint. */
	UPROPERTY(EditAnywhere, Category = "Damage", meta = (DisplayName = "Event Name"))
		FName m_EventName;

	/* What realm, server or client, to execute this task. If your game isn't networked - this field is ignored. */
	UPROPERTY(EditAnywhere, Category = "Realm", meta = (DisplayName = "Realm"))
		TEnumAsByte<EAblAbilityTaskRealm> m_TaskRealm;

	/* If true, we will use the Async graph to calculate damage for all Targets across multiple cores. This can speed up execution if the ability
	*  affects a large number of targets and/or the calculations for damage require extensive checks. */
	UPROPERTY(EditAnywhere, Category = "Damage|Optimization", meta = (DisplayName = "Use Async Calculate"))
		bool m_UseAsyncCalculate;
};

#undef LOCTEXT_NAMESPACE
