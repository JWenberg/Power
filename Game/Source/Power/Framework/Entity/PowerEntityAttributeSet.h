 

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
    UFUNCTION() void OnRep_Health() { GAMEPLAYATTRIBUTE_REPNOTIFY(UPowerEntityAttributeSet, Health) UpdateAttributeDisplays(); }

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_MaxHealth)
    FGameplayAttributeData MaxHealth;
    DECLARE_ATTRIBUTE_FUNCTION(MaxHealth)
    UFUNCTION() void OnRep_MaxHealth() { GAMEPLAYATTRIBUTE_REPNOTIFY(UPowerEntityAttributeSet, MaxHealth) UpdateAttributeDisplays(); }

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_Mana)
    FGameplayAttributeData Mana;
    DECLARE_ATTRIBUTE_FUNCTION(Mana)
    UFUNCTION() void OnRep_Mana() { GAMEPLAYATTRIBUTE_REPNOTIFY(UPowerEntityAttributeSet, Mana) UpdateAttributeDisplays(); }

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_MaxMana)
    FGameplayAttributeData MaxMana;
    DECLARE_ATTRIBUTE_FUNCTION(MaxMana)
	UFUNCTION() void OnRep_MaxMana() { GAMEPLAYATTRIBUTE_REPNOTIFY(UPowerEntityAttributeSet, MaxMana) UpdateAttributeDisplays();}


    virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData &Data) override;

	//Updates the nameplate and HUD (if a player) with updated attributes.
	void UpdateAttributeDisplays();
};
