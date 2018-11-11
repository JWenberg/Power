// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "RotateNameplateToCamera.generated.h"

/**
 * 
 */
UCLASS()
class POWER_API URotateNameplateToCamera : public UWidgetComponent
{
	GENERATED_BODY()
	
public:
	URotateNameplateToCamera();

	//config Variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
		bool AutoActivateWidgetDisplay;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
		bool UsePlayerFacingMode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
		float RotationControlTimerInterval;

	//Runtime Variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Runtime")
		bool WidgetDisplayActivated;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Runtime")
		FTimerHandle RotationControlTimer;

	UFUNCTION(BlueprintCallable)
		void ActivateWidgetDisplay();

	UFUNCTION(BlueprintCallable)
		void DeactivateWidgetDisplay();

	UFUNCTION(BlueprintCallable)
		void OrientTowardsPlayerCamera();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	
	
};
