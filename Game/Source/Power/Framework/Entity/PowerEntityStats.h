// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#define DECLARE_POWER_STAT(StatName) UFUNCTION(BlueprintCallable) UPowerStat* StatName##Stat() { return StatName; }

#include "CoreMinimal.h"
#include "PowerEntity.h"
#include "PowerEntityStats.generated.h"

UCLASS()
class POWER_API UPowerStat : public UObject 
{
    GENERATED_BODY()

private:
    UPROPERTY(VisibleAnywhere)
    float BaseValue;

    UPROPERTY(VisibleAnywhere)
    float CurrentValue;

    UPROPERTY(VisibleAnywhere)
    float MinValue;

    UPROPERTY(VisibleAnywhere)
    float MaxValue;

    bool isClamped;

    //TODO : Add an array of modifiers (flat & percent) that'll be used for buffs
    //Create a Map<PowerStat, List<StatModifier>> 
    //StatModifier:
    //  bool isFlat; defines if it's a flat increase or a percent increase
    //  float Amount; defines the amount
public:
    UPowerStat();
    UPowerStat(float Base);
    UPowerStat(float Base, float Min, float Max);

    ~UPowerStat();

    UFUNCTION(BlueprintCallable)
    void Add(float Val);

    UFUNCTION(BlueprintCallable)
    void Sub(float Val);

    UFUNCTION(BlueprintCallable)
    void Set(float Val);

    float GetCurrentValue();
    float GetMaxValue();
    float GetMinValue();
};

UCLASS()
class POWER_API UPowerEntityStats : public UObject
{
    GENERATED_BODY()
public:
    UPowerEntityStats();
    ~UPowerEntityStats();

    APowerEntity* Owner;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, ReplicatedUsing=OnRep_Health)
    UPowerStat* Health;
    DECLARE_POWER_STAT(Health)
    UFUNCTION() void OnRep_Health() { UpdateDisplays(); };
    
    void UpdateDisplays();
};
