// Fill out your copyright notice in the Description page of Project Settings.

#include "RotateNameplateToCamera.h"
#include "Classes/Kismet/GameplayStatics.h"
#include "Classes/Kismet/KismetMathLibrary.h"

URotateNameplateToCamera::URotateNameplateToCamera() {
	this->WidgetDisplayActivated = false;
	this->UsePlayerFacingMode = true;
	this->AutoActivateWidgetDisplay = true;
	this->RotationControlTimerInterval = 0.005;
	this->bEditableWhenInherited = true;

}

// Called when the game starts or when spawned
void URotateNameplateToCamera::BeginPlay()
{
	Super::BeginPlay();
	if (AutoActivateWidgetDisplay == true) {
		ActivateWidgetDisplay();
	} else if(AutoActivateWidgetDisplay == false){
		DeactivateWidgetDisplay();
	}
}

void URotateNameplateToCamera::ActivateWidgetDisplay() {
	if (WidgetDisplayActivated == false) {
		if (UsePlayerFacingMode == true) {
			OrientTowardsPlayerCamera();
			this->SetHiddenInGame(false);
			GetWorld()->GetTimerManager().SetTimer(RotationControlTimer, this, &URotateNameplateToCamera::OrientTowardsPlayerCamera, RotationControlTimerInterval, true);
			WidgetDisplayActivated = true;
		}
	}
}

void URotateNameplateToCamera::DeactivateWidgetDisplay() {
	if (this->WidgetDisplayActivated == true) {
		this->SetHiddenInGame(true);
		GetWorld()->GetTimerManager().ClearTimer(RotationControlTimer);
		this->WidgetDisplayActivated = false;
	}
}

void URotateNameplateToCamera::OrientTowardsPlayerCamera() {
	if (GetOwner()->WasRecentlyRendered() == true) {
		APlayerCameraManager* CameraManager = Cast<APlayerCameraManager>(UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0));
		FRotator LookAt = UKismetMathLibrary::FindLookAtRotation(GetOwner()->GetActorLocation(), CameraManager->GetCameraLocation());
		SetWorldRotation(LookAt);
	}
	
}