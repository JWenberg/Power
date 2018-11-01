// Fill out your copyright notice in the Description page of Project Settings.

#include "MMO_GameMode.h"
#include "Power/GameState/MMO_GameState.h"

AMMO_GameMode::AMMO_GameMode()
{
	//Set our defualt Gamestate
	GameStateClass = AMMO_GameState::StaticClass();
}


