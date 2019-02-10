 

#include "MMO_PlayerController.h"
#include "SpatialNetDriver.h"
#include "EntityRegistry.h"

AMMO_PlayerController::AMMO_PlayerController() { 
}

void AMMO_PlayerController::Sort_Target_Array() {
	TArray<float> target_distances;
	int numTargets = target_array_.Num();	
	target_distances.SetNumUninitialized(numTargets, true);
	
	FVector player_location = this->GetPawn()->GetActorLocation();
	UE_LOG(LogTemp, Warning, TEXT("%s is at %s"), *GetNameSafe(GetPawn()), *player_location.ToString());

	//Calculate the distance between the player and each of the targets in the target array
	for (int i = 0; i < numTargets; i++) {
		float distance = (player_location - (target_array_[i]->GetActorLocation())).Size();
		target_distances[i] = distance;
	}

	//Sort the Target array from closest target to furthest.
	for (int j = 0; j < numTargets -1; j++) {
		for (int y = 0; y < numTargets - 1; y++) {
			if (target_distances[y] > target_distances[y + 1]) {
				float temp_f = target_distances[y];
				APowerEntity* temp_p = target_array_[y];
				
				target_distances[y] = target_distances[y+1];
				target_array_[y] = target_array_[y+1];

				target_distances[y+1] = temp_f;
				target_array_[y+1] = temp_p;

				UE_LOG(LogTemp, Warning, TEXT("%s is %f from Player"), *GetNameSafe(target_array_[y]), target_distances[y]);
				UE_LOG(LogTemp, Warning, TEXT("%s is %f from Player"), *GetNameSafe(target_array_[y+1]), target_distances[y+1]);
			}
		}
	}
	

	//Target's should now be sorted by distance (index 0 is closest), so set player's target to the closest target.
	APowerEntity* controlled_pawn  = (APowerEntity*)this->GetPawn();
	controlled_pawn->SetTargetAndHandleCircle(target_array_[0]);

	for (int x = 0; x < numTargets; x++) {
		UE_LOG(LogTemp, Warning, TEXT("%s is %f from Player"), *GetNameSafe(target_array_[x]), target_distances[x]);
	}

}