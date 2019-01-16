// Fill out your copyright notice in the Description page of Project Settings.

#include "PowerTargeting_ProjectileTarget.h"

void UPowerTargeting_ProjectileTarget::FindTargets(UAblAbilityContext & Context) const {
	APower_ProjectileActor* Projectile = Cast<APower_ProjectileActor>(Context.GetInstigator());
	check(Projectile);

	if (Projectile) {
		Context.GetMutableTargetActors().Add(Projectile->TargetEntity);
	}
}


