// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PowerEntity.generated.h"

UCLASS()
class POWER_API APowerEntity : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APowerEntity();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;
	
// PowerEntity framework
public:

    /* Health related things */
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Entity stats", Replicated)
    int Health;

    UFUNCTION(BlueprintCallable)
    void DealDamage(int Amount);

    UFUNCTION(Server, Reliable, WithValidation)
    void ServerDealDamage(int Amount);

    virtual void ServerDealDamage_Implementation(int Amount);
    virtual bool ServerDealDamage_Validate(int Amount);

    /* Mana related things */
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Entity stats", Replicated)
    int Mana; 

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Entity stats", Replicated)
    int Level;

    APowerEntity* Target;

// Rendering stuff
public:
    UDecalComponent* TargetCircle;
};
