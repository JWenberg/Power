// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MMO_GameMode.generated.h"

/**
 * 
 */
UCLASS(SpatialType = (Singleton, ServerOnly))
class POWER_API AMMO_GameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AMMO_GameMode();
	
	
};
