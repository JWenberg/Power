 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "Abilities/GameplayAbility.h"
#include "UI/Nameplates/NameplateController.h"
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
    UFUNCTION(BlueprintCallable)
    int GetHealth();

    UFUNCTION(BlueprintCallable)
    int GetMaxHealth();

    /* Mana related things */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Entity stats", Replicated)
    int Mana; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Entity stats", Replicated)
	int MaxMana;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Entity stats", Replicated)
    int Level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Entity stats", Replicated)
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Entity stats", Replicated)
	FString Guild;

/* Targeting */

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Entity", Replicated)
    APowerEntity* TargetEntity;

    UFUNCTION(BlueprintCallable)
    void SetTargetAndHandleCircle(APowerEntity* EntityToTarget);

    UFUNCTION(BlueprintCallable)
    void ChangeTarget(APowerEntity* NewTarget);

    UFUNCTION(Server, Reliable, WithValidation)
    void ServerChangeTarget(APowerEntity* NewTarget);

    virtual void ServerChangeTarget_Implementation(APowerEntity* NewTarget);
    virtual bool ServerChangeTarget_Validate(APowerEntity* NewTarget);

/* Casting abilities */

    UFUNCTION(BlueprintCallable)
    void CastAbilityOnTarget(TSubclassOf<UGameplayAbility> AbilityToCast, FGameplayTag EventTag);

    UFUNCTION(Server, Reliable, WithValidation)
    void ServerCastAbilityOnTarget(TSubclassOf<UGameplayAbility> AbilityToCast, FGameplayTag EventTag);

    virtual void ServerCastAbilityOnTarget_Implementation(TSubclassOf<UGameplayAbility> AbilityToCast, FGameplayTag EventTag);
    virtual bool ServerCastAbilityOnTarget_Validate(TSubclassOf<UGameplayAbility> AbilityToCast, FGameplayTag EventTag);
public:
	// Rendering stuff
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UDecalComponent* TargetCircle;

	//Nameplate stuff
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UNameplateController* NameplateController;

	//Update the player's hud
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateHUD();
};
