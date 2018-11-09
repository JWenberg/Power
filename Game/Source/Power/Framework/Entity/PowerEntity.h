// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "Abilities/GameplayAbility.h"
#include "PowerEntity.generated.h"

UCLASS(config = Game)
class POWER_API APowerEntity : public ACharacter, public IAbilitySystemInterface
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


/*     Gameplay Ability System      */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Abilities, meta = (AllowPrivateAccess = "true"))
    class UAbilitySystemComponent* AbilitySystem;

    UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilitySystem; };
    
    /* Gives the ability to the Entity */
    UFUNCTION(BlueprintCallable, Category = "Abilities")
    void GiveAbility(TSubclassOf<UGameplayAbility> AbilityToGive);

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    class UPowerEntityAttributeSet* AttributeSet;

    /* Health related things */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Entity stats", Replicated)
    int Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Entity stats", Replicated)
	int MaxHealth;

    UFUNCTION(BlueprintCallable)
    void DealDamage(int Amount);

    UFUNCTION(Server, Reliable, WithValidation)
    void ServerDealDamage(int Amount);

    virtual void ServerDealDamage_Implementation(int Amount);
    virtual bool ServerDealDamage_Validate(int Amount);

    /* Mana related things */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Entity stats", Replicated)
    int Mana; 

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Entity stats", Replicated)
    int Level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Entity stats", Replicated)
	FString Name;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Entity", Replicated)
    APowerEntity* TargetEntity;

// Rendering stuff
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UDecalComponent* TargetCircle;
};
