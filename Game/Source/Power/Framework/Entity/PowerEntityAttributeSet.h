// Fill out your copyright notice in the Description page of Project Settings.

#pragma once



#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "PowerEntityAttributeSet.generated.h"

//This macro should be used in .H files
#define DECLARE_ATTRIBUTE_FUNCTION(PropertyName) static FGameplayAttribute PropertyName##Attribute();

//This macro should be used in .CPP files
#define DEFINE_ATTRIBUTE_FUNCTION(PropertyName, ClassName) 																							\
FGameplayAttribute ClassName##::PropertyName##Attribute()																								\
{																																						\
	static UProperty* Property = FindFieldChecked<UProperty>(ClassName##::StaticClass(), GET_MEMBER_NAME_CHECKED(ClassName, PropertyName));				\
	return FGameplayAttribute(Property);																												\
}

/**
 * 
 */
UCLASS()
class POWER_API UPowerEntityAttributeSet : public UAttributeSet
{
    GENERATED_BODY()

    UPROPERTY()
    FGameplayAttributeData Health;
	
    DECLARE_ATTRIBUTE_FUNCTION(Health)
	
};
