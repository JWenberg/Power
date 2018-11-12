 

#pragma once

#include "CoreMinimal.h"
#include "EntityRegistry.h"
#include "GameFramework/Character.h"
#include "Power/Framework/Entity/PowerEntity.h"
#include "MMO_Player.generated.h"

UCLASS(config = Game, SpatialType)
class POWER_API AMMO_Player : public APowerEntity
{
	GENERATED_BODY()
public:
	AMMO_Player();

	virtual void BeginPlay() override;

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
    void Interact();
};