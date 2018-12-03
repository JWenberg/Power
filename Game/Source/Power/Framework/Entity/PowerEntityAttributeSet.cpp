 

#include "PowerEntityAttributeSet.h"
#include "PowerEntity.h"
#include "GameplayEffectExtension.h"


DEFINE_ATTRIBUTE_FUNCTION(Health, UPowerEntityAttributeSet)
DEFINE_ATTRIBUTE_FUNCTION(MaxHealth, UPowerEntityAttributeSet)
DEFINE_ATTRIBUTE_FUNCTION(Mana, UPowerEntityAttributeSet)
DEFINE_ATTRIBUTE_FUNCTION(MaxMana, UPowerEntityAttributeSet)

UPowerEntityAttributeSet::UPowerEntityAttributeSet() : Health(1000.f), MaxHealth(1000.f)
{

}

void UPowerEntityAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    DOREPLIFETIME_CONDITION_NOTIFY(UPowerEntityAttributeSet, Health, COND_None, REPNOTIFY_Always)
    DOREPLIFETIME_CONDITION_NOTIFY(UPowerEntityAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always)
    DOREPLIFETIME_CONDITION_NOTIFY(UPowerEntityAttributeSet, Mana, COND_None, REPNOTIFY_Always)
    DOREPLIFETIME_CONDITION_NOTIFY(UPowerEntityAttributeSet, MaxMana, COND_None, REPNOTIFY_Always)
}

void UPowerEntityAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData & Data)
{
}

void UPowerEntityAttributeSet::UpdateAttributeDisplays() {
	APowerEntity* Owner = Cast<APowerEntity>(this->GetOwningActor());
	Owner->GetName();
	if (Owner) {
		//Update the entity's nameplate
		Owner->NameplateController->UpdateNameplate();
		//UE_LOG(LogTemp, Warning, TEXT("Nameplate Should Update"));

		//If entity is a player with a hud, update it's hud as well
		if (Owner->IsPlayerControlled()) {
			Owner->UpdateHUD();
		}
	}
}