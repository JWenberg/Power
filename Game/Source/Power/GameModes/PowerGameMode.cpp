// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "PowerGameMode.h"
#include "Player/MMO_Player.h"
#include "Player/MMO_PlayerController.h"
#include "PlayerStates/MMO_PlayerState.h"
#include "GameStates/MMO_GameState.h"
#include "UObject/ConstructorHelpers.h"

APowerGameMode::APowerGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Power/Character/Player/BP_MMOPlayer"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	PlayerControllerClass = AMMO_PlayerController::StaticClass();
	GameStateClass = AMMO_GameState::StaticClass();
	PlayerStateClass = AMMO_PlayerState::StaticClass();
}
