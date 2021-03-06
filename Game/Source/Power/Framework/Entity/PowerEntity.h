 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FloatAttr.h"
#include "UI/Nameplates/NameplateController.h"
#include "PowerEntity.generated.h"

UCLASS(config = Game)
class POWER_API APowerEntity : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APowerEntity();

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	
	/* Attribute Stuff */
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Player Attributes", Replicated)
	FFloatAttr entity_health;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Player Attributes", Replicated)
	FFloatAttr entity_mana;

	UFUNCTION(BlueprintCallable)
	void PowerTakeDamage(float damage);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerPowerTakeDamage(float damage);

	virtual void ServerPowerTakeDamage_Implementation(float damage);
	virtual bool ServerPowerTakeDamage_Validate(float damage);

    /* Non-AttributeSet Attributes */
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

	/* Skillshot Targeting */ 
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Skillshots", Replicated)
	FVector skillshot_location;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Skillshots", Replicated)
	FName skillshot_name;

	/* Component References */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UDecalComponent* TargetCircle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UNameplateController* NameplateController;

	/* UMG Stuff */
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateHUD();
};
