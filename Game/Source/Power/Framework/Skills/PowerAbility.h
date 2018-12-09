// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Abilities/GameplayAbility.h"
#include "PowerAbility.generated.h"

/**
 * 
 */
UCLASS()
class POWER_API UPowerAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UPowerAbility();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Properties")
	FString AbilityName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Properties")
	bool bIsProjectileAbility;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Properties")
	bool bPlaysAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Properties")
	bool bTargetIsSelf;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Properties")
	bool bIsAOE;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Properties")
	int iEffectDelay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Properties")
	int iProjectileOriginType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Properties")
	int iProjectileType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Properties")
	int iDamageType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Properties")
	int iAOERange;

};
