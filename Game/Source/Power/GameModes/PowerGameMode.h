// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PowerGameMode.generated.h"

UCLASS(minimalapi, SpatialType = (Singleton, ServerOnly))
class APowerGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	APowerGameMode();
};



