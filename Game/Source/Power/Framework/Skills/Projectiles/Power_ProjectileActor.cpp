// Fill out your copyright notice in the Description page of Project Settings.

#include "Power_ProjectileActor.h"


// Sets default values
APower_ProjectileActor::APower_ProjectileActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APower_ProjectileActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APower_ProjectileActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

