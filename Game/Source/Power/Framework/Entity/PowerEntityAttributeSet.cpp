// Fill out your copyright notice in the Description page of Project Settings.

#include "PowerEntityAttributeSet.h"


DEFINE_ATTRIBUTE_FUNCTION(Health, UPowerEntityAttributeSet)

UPowerEntityAttributeSet::UPowerEntityAttributeSet() : Health(1000.f)
{

}

void UPowerEntityAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    DOREPLIFETIME_CONDITION_NOTIFY(UPowerEntityAttributeSet, Health, COND_None, REPNOTIFY_Always)
}