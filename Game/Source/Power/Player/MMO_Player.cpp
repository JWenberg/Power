 

#include "MMO_Player.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "UnrealNetwork.h"
#include "SpatialNetDriver.h"

// Sets default values
AMMO_Player::AMMO_Player() : APowerEntity()
{

}

void AMMO_Player::BeginPlay()
{
	Super::BeginPlay();
}

// Called to bind functionality to input
void AMMO_Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up movement key bindings
	check(PlayerInputComponent);
    PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AMMO_Player::Interact);
	
    //AbilitySystem->BindAbilityActivationToInputComponent(PlayerInputComponent, FGameplayAbiliyInputBinds("ConfirmInput", "CancelInput", "AbilityInput"));
}

void AMMO_Player::Interact()
{
    // Note:
    // Interact is, for now, used as a test method for pretty much anything we wanna try out. 
    // In this case, pressing F will deal damage to the character pressing it. 
    // Allows checking that health is replicated to everyone
    UE_LOG(LogTemp, Warning, TEXT("Health should decrease"))
}
