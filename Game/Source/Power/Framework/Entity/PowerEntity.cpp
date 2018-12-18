 

#include "PowerEntity.h"
#include "UnrealNetwork.h"
#include "PowerEntityAttributeSet.h"
#include "PowerEntityStats.h"
#include "Classes/Components/DecalComponent.h"


// Sets default values
APowerEntity::APowerEntity() {
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//AttributeSet Initialization
	this->AttributeSet = CreateDefaultSubobject<UPowerEntityAttributeSet>(TEXT("PowerEntityAttributeSet"));

	//Default Values for non-AttributeSet Attributes
	this->Level = 1;
	this->Name = "Jansen";
	this->Guild = "IsInAGuild";

	//Nameplate Component Initialization
	static ConstructorHelpers::FClassFinder<UNameplateController> NameplateReference(TEXT("/Game/Power/UI/NamePlates/BP_NameplateController"));
	if (NameplateReference.Succeeded()) {
		this->NameplateController = static_cast<UNameplateController*>(CreateDefaultSubobject(TEXT("NameplateController"), UNameplateController::StaticClass(), NameplateReference.Class.Get(), true, false, false));
		this->NameplateController->SetupAttachment(RootComponent);
		this->NameplateController->SetRelativeLocation(FVector(0, 0, 120));
		this->NameplateController->UpdateNameplate();
	}

	//TargetCircle Component Initialization
	this->TargetCircle = CreateDefaultSubobject<UDecalComponent>(TEXT("TargetCircle"));
	this->TargetCircle->DecalSize = FVector(64.f, 64.f, 64.f);
	this->TargetCircle->SetupAttachment(RootComponent);
	this->TargetCircle->SetRelativeRotation(FQuat(180.f, 90.f, 180.f, 0.0f));
	this->TargetCircle->SetRelativeLocation(FVector(0.f, 0.f, -110.f));
	this->TargetCircle->SetVisibility(false);
	static ConstructorHelpers::FObjectFinder<UMaterial> DecalMaterial(TEXT("Material'/Game/Power/Materials/TargetDecal/M_DecalTarget.M_DecalTarget'"));
	if (DecalMaterial.Succeeded()) {
		this->TargetCircle->SetMaterial(0, DecalMaterial.Object);
	}

}

//================================================================================
// UE4 Stuff
//================================================================================

// Called when the game starts or when spawned
void APowerEntity::BeginPlay() {
	Super::BeginPlay();
	UpdateHUD();
}

//Replication for non-AttributeSet Attributes
void APowerEntity::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const {

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APowerEntity, Name);
	DOREPLIFETIME(APowerEntity, Level);
	DOREPLIFETIME(APowerEntity, TargetEntity);
}

//================================================================================
// Attribute Stuff
//================================================================================

int APowerEntity::GetHealth() {
	return AttributeSet->Health.GetCurrentValue();
}

int APowerEntity::GetMaxHealth() {
	return AttributeSet->MaxHealth.GetCurrentValue();
}

int APowerEntity::GetMana() {
	return AttributeSet->Mana.GetCurrentValue();
}

int APowerEntity::GetMaxMana() {
	return AttributeSet->MaxMana.GetCurrentValue();
}

void APowerEntity::ReduceHealth(float HealthToReduce) {
	float newHealthValue = (float)GetHealth() - HealthToReduce;
	
	//Make sure health value never goes below 0
	if (newHealthValue <= 0) {
		newHealthValue = 0;
		//Todo: Die
	} 
	AttributeSet->Health.SetCurrentValue(newHealthValue);
}

//================================================================================
// Targeting Stuff
//================================================================================

void APowerEntity::SetTargetAndHandleCircle(APowerEntity * EntityToTarget) {
	if (this->TargetEntity) {
		this->TargetEntity->TargetCircle->SetVisibility(false);
	}

	this->ChangeTarget(EntityToTarget);

	if (EntityToTarget) {
		EntityToTarget->TargetCircle->SetVisibility(true);
	}
	UpdateHUD();
}

void APowerEntity::ChangeTarget(APowerEntity* NewTarget) {
	if (Role < ROLE_Authority) {
		ServerChangeTarget(NewTarget);
	}

	this->TargetEntity = NewTarget;
}

void APowerEntity::ServerChangeTarget_Implementation(APowerEntity* NewTarget) {
	ChangeTarget(NewTarget);
}

bool APowerEntity::ServerChangeTarget_Validate(APowerEntity* NewTarget) {
	return true;
}

//================================================================================
// Ability Stuff
//================================================================================

//================================================================================
// UMG Stuff
//================================================================================