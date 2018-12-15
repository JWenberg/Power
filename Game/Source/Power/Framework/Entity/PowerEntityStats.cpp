// Fill out your copyright notice in the Description page of Project Settings.

#include "PowerEntityStats.h"
#include "UnrealNetwork.h"

UPowerEntityStats::UPowerEntityStats()
{
    this->Health = NewObject<UPowerStat>(this, TEXT("Health"));
}

UPowerEntityStats::~UPowerEntityStats()
{
}

void UPowerEntityStats::UpdateDisplays()
{
    if (Owner) {
        //Update the entity's nameplate
        Owner->NameplateController->UpdateNameplate();
        //UE_LOG(LogTemp, Warning, TEXT("Nameplate Should Update"));

        //If entity is a player with a hud, update it's hud as well
        if (Owner->IsPlayerControlled()) {
            Owner->UpdateHUD();
        }
    }
}

void UPowerEntityStats::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    DOREPLIFETIME(UPowerEntityStats, Health)
}

UPowerStat::UPowerStat()
{
    this->BaseValue = FMath::RandRange(1000, 2000);
    this->CurrentValue = this->BaseValue;
    this->MinValue = 0;
    this->MaxValue = 2000;
    this->isClamped = true;
}

UPowerStat::UPowerStat(float Base)
{
    this->BaseValue = Base;
    this->CurrentValue = this->BaseValue;
    this->isClamped = false;
}

UPowerStat::UPowerStat(float Base, float Min, float Max)
{
    this->BaseValue = Base;
    this->CurrentValue = this->BaseValue;
    this->MinValue = Min;
    this->MaxValue = Max;
    this->isClamped = true;
}

UPowerStat::~UPowerStat()
{
}

void UPowerStat::Add(float Val)
{
    this->CurrentValue += Val;
    if (this->isClamped && this->CurrentValue > this->MaxValue) {
        this->CurrentValue = this->MaxValue;
    }
}

void UPowerStat::Sub(float Val)
{
    this->CurrentValue -= Val;
    if (this->isClamped && this->CurrentValue < this->MinValue) {
        this->CurrentValue = this->MinValue;
    }
}

void UPowerStat::Set(float Val)
{
    this->CurrentValue = Val;
    if (this->isClamped) {
        if (this->CurrentValue < this->MinValue) {
            this->CurrentValue = this->MinValue;
        } else if (this->CurrentValue > this->MaxValue) {
            this->CurrentValue = this->MaxValue;
        }
    }
}

float UPowerStat::GetCurrentValue()
{
    return this->CurrentValue;
}

float UPowerStat::GetMaxValue()
{
    return this->MaxValue;
}

float UPowerStat::GetMinValue()
{
    return this->MinValue;
}
