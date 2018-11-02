// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EntityRegistry.h"
#include "GameFramework/Character.h"
#include "MMO_Player.generated.h"

UCLASS(config = Game, SpatialType)
class POWER_API AMMO_Player : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
public:
	AMMO_Player();

	virtual void BeginPlay() override;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseLookUpRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool LeftMouseHeld;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool RightMouseHeld;

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerTurnPlayer(FRotator NewRot);

	virtual void ServerTurnPlayer_Implementation(FRotator NewRot);
	virtual bool ServerTurnPlayer_Validate(FRotator NewRot);

protected:
	//Movement Controls
	void MoveForward(float Value);
	void MoveBackward(float Value);
	
	void RotateRight(float Value);
	void RotateLeft(float Value);
	
	void StrafeRight(float Value);
	void StrafeLeft(float Value);

	void UpdatePlayerRot(FRotator NewRot);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

};