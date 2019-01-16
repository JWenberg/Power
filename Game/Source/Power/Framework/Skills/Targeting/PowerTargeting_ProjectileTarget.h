// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Targeting/ablTargetingBase.h"
#include "Framework/Skills/Projectiles/Power_ProjectileActor.h"
#include "PowerTargeting_ProjectileTarget.generated.h"

/**
 * 
 */
UCLASS()
class POWER_API UPowerTargeting_ProjectileTarget : public UAblTargetingBase
{
	GENERATED_BODY()

public:
	/*
	Get the homing projectile's target and set it as the target for the damaging ability
	*/
	virtual void FindTargets(UAblAbilityContext& Context) const override;
	
};
