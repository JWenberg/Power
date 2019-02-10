 

#pragma once

#include "CoreMinimal.h"
#include "Power/Framework/Entity/PowerEntity.h"
#include "GameFramework/PlayerController.h"
#include "MMO_PlayerController.generated.h"

/**
 * 
 */
UCLASS(SpatialType)
class POWER_API AMMO_PlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:		
	AMMO_PlayerController();

	UFUNCTION(BlueprintCallable)
	void Sort_Target_Array();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<APowerEntity*> target_array_;


};
