// Fill out your copyright notice in the Description page of Project Settings.

#pragma once



#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "UnrealNetwork.h"
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

#define DECLARE_ATTRIBUTE_REPLICATION(PropertyName) UFUNCTION() void OnRep_##PropertyName() { GAMEPLAYATTRIBUTE_REPNOTIFY(UPowerEntityAttributeSet, PropertyName) }
/**
 * 
 */
UCLASS()
class POWER_API UPowerEntityAttributeSet : public UAttributeSet
{
    GENERATED_BODY()

public:
    UPowerEntityAttributeSet();

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_Health)
        FGameplayAttributeData Health;
    DECLARE_ATTRIBUTE_FUNCTION(Health)
    UFUNCTION() void OnRep_Health() { GAMEPLAYATTRIBUTE_REPNOTIFY(UPowerEntityAttributeSet, Health) }
    //DECLARE_ATTRIBUTE_REPLICATION(Health)
};
