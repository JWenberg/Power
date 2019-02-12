 

#pragma once

#include "CoreMinimal.h"
#include "Power/Framework/Entity/PowerEntity.h"
#include "GameFramework/PlayerController.h"
#include "MMO_PlayerController.generated.h"

/**
 * 
 */
USTRUCT()
struct FTargetStruct {
	GENERATED_BODY()

public:
	FTargetStruct();
	FTargetStruct(APowerEntity* a) {
		target_ = a;
		b_iterated_ = false;
		is_in_range_ = true;
	}

	float distance_to_player;		//Distance from player to target
	APowerEntity* target_;			//Target Reference
	bool b_iterated_;				//Bool for telling if we have iterated over this target yet
	bool is_in_range_;				//For removing targets that aren't in range of the player anymore

};

UCLASS(SpatialType)
class POWER_API AMMO_PlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:		
	AMMO_PlayerController();

	UFUNCTION(BlueprintCallable)
	void Sort_Target_Array();

	UFUNCTION(BlueprintCallable)
	void Clear_Target_Array();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<APowerEntity*> target_array_;

	UPROPERTY()
	TArray<FTargetStruct> target_struct_array_;



};
