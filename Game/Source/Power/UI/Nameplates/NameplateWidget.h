// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NameplateWidget.generated.h"

/**
 * 
 */
UCLASS()
class POWER_API UNameplateWidget : public UUserWidget
{
	GENERATED_BODY()

public:	

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Output")
	void UpdateName(const FString& Name);
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Output")
	void UpdateLevel(const int& Level);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Output")
	void UpdateHealthBar(const int& CurrentHealth, const int& NewHealth);

};
