 

#include "MMO_PlayerController.h"
#include "SpatialNetDriver.h"
#include "EntityRegistry.h"

AMMO_PlayerController::AMMO_PlayerController() { 

}

FTargetStruct::FTargetStruct() {

}

void AMMO_PlayerController::Sort_Target_Array() {
	int numTargets = target_array_.Num();	
	
	FVector player_location = this->GetPawn()->GetActorLocation();

	//Add the targets returned by the sphere query to the struct array if they don't already exist
	for (int i = 0; i < numTargets; i++) {
		bool in_struct_array = false;
		for (int p = 0; p < target_struct_array_.Num(); p++) {
			if (target_struct_array_[p].target_ == target_array_[i]) {
				target_struct_array_[p].is_in_range_ = true;
				//Update the distance between the player and the target
				target_struct_array_[p].distance_to_player = (player_location - (target_array_[i]->GetActorLocation())).Size();
				in_struct_array = true;
			}
		}

		if (in_struct_array == false) {
			FTargetStruct target(target_array_[i]);
			target.distance_to_player = (player_location - (target_array_[i]->GetActorLocation())).Size();

			target_struct_array_.Add(target);
		}
	}

	//Remove any old target's that aren't in range anymore
	for (int j = 0; j < target_struct_array_.Num(); j++) {
		if (target_struct_array_[j].is_in_range_ == false) {
			target_struct_array_.RemoveAt(j, 1, true);
		} else {
			//Set to false for when next time tab is pressed.
			//If the target is still in range, it will be set to true in the loop above
			target_struct_array_[j].is_in_range_ = false;
		}
	}


	//Sort the struct array by distance
	for (int j = 0; j < target_struct_array_.Num(); j++) {
		for (int y = 0; y < target_struct_array_.Num() - j -1; y++) {
			if (target_struct_array_[y].distance_to_player > target_struct_array_[y+1].distance_to_player) {
				FTargetStruct temp = target_struct_array_[y];
				
				target_struct_array_[y] = target_struct_array_[y+1];

				target_struct_array_[y+1] = temp;
			}
		}
	}
	
	//Find the first target not iterated over yet, if all target's have been iterated over 
	//then set iterated to false for each of them and select the closest target.
	bool iterated_all = true;
	for (int a = 0; a < target_struct_array_.Num() && iterated_all == true; a++) {
		if (target_struct_array_[a].b_iterated_ == false) {
			((APowerEntity*)GetPawn())->SetTargetAndHandleCircle(target_struct_array_[a].target_);
			target_struct_array_[a].b_iterated_ = true;
			iterated_all = false;
		}
	}

	//If we have iterated over all of the targets in the struct array, then set all b_iterated to false and target the closest target
	if (iterated_all == true) {
		for (int v = 0; v < target_struct_array_.Num(); v++) {
			target_struct_array_[v].b_iterated_ = false;
		}

		((APowerEntity*)GetPawn())->SetTargetAndHandleCircle(target_struct_array_[0].target_);
		target_struct_array_[0].b_iterated_ = true;
	}

}

void AMMO_PlayerController::Clear_Target_Array() {
	target_struct_array_.Empty();
}