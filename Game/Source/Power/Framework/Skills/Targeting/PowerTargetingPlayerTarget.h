// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Entity/PowerEntity.h"
#include "Targeting/ablTargetingBase.h"
#include "PowerTargetingPlayerTarget.generated.h"

UCLASS(EditInlineNew, meta = (DisplayName = "Player Target", ShortToolTip = "The player's target."))
class UPowerTargetingPlayerTarget : public UAblTargetingBase
{
    GENERATED_BODY()
public:
    UPowerTargetingPlayerTarget(const FObjectInitializer& ObjectInitializer);
    virtual ~UPowerTargetingPlayerTarget();

    /* Find the player's target. */
    virtual void FindTargets(UAblAbilityContext& Context) const override;
private:

    /* Range of the spell. */
    UPROPERTY(EditInstanceOnly, Category = "Player Target", meta = (DisplayName = "Range", ClampMin = 0.1f))
    float Range;
};