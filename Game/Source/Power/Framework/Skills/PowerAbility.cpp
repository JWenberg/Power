// Fill out your copyright notice in the Description page of Project Settings.

#include "PowerAbility.h"



UPowerAbility::UPowerAbility() {
	//Define Class Defaults
	this->AbilityName = "GenericAbility";
	this->bIsProjectileAbility = false;
	this->bTargetIsSelf = false;
	this->bIsAOE = false;
	this->iEffectDelay = 0;
	this->iProjectileOriginType = 0;
	this->iProjectileType = 0;
	this->iDamageType = 0;
	this->iAOERange = 0;
}

