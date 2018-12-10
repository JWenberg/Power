// Fill out your copyright notice in the Description page of Project Settings.

#include "PowerTargetingPlayerTarget.h"

UPowerTargetingPlayerTarget::UPowerTargetingPlayerTarget(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer),
    Range(1.0f)
{

}

UPowerTargetingPlayerTarget::~UPowerTargetingPlayerTarget()
{
}

void UPowerTargetingPlayerTarget::FindTargets(UAblAbilityContext & Context) const
{
    AActor* SourceActor = m_Location.GetSourceActor(Context);
    APowerEntity* SourcePowerActor = Cast<APowerEntity>(SourceActor);
    check(SourcePowerActor);

    if (APowerEntity* TargetActor = SourcePowerActor->TargetEntity) {
        if (TargetActor->GetDistanceTo(SourceActor) <= Range) {
            Context.GetMutableTargetActors().Add(TargetActor);
        }
    }
}
