 

#include "PowerEntity.h"
#include "UnrealNetwork.h"
#include "Classes/Components/DecalComponent.h"


// Sets default values
APowerEntity::APowerEntity() {
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Default Values for non-AttributeSet Attributes
	this->Level = 1;
	this->Name = "Jansen";
	this->Guild = "IsInAGuild";

	//Nameplate Component Initialization
	static ConstructorHelpers::FClassFinder<UNameplateController> NameplateReference(TEXT("/Game/Power/UI/NamePlates/BP_NameplateController"));
	if (NameplateReference.Succeeded()) {
		FName component_unique_name = FName(*GetNameSafe(this).Append("_N_Controller"));
		this->NameplateController = (UNameplateController*)(CreateDefaultSubobject(component_unique_name ,UNameplateController::StaticClass(), NameplateReference.Class.Get(), true, false, false));
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

	//Set Attribute Base Stats
	entity_health.SetBaseValue(100);
	entity_mana.SetBaseValue(1000);

}

//================================================================================
// UE4 Stuff
//================================================================================

// Called when the game starts or when spawned
void APowerEntity::BeginPlay() {
	Super::BeginPlay();
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


//Damage Methods
void APowerEntity::PowerTakeDamage(float damage) {
	//TODO: Faction check damage dealer
	if (Role < ROLE_Authority) {
		ServerPowerTakeDamage(damage);
	}

	entity_health = (entity_health.GetValue() - damage);

	if (entity_health.GetValue() <= 0) {
		entity_health = 0;
		//TODO: die
	}
}

void APowerEntity::ServerPowerTakeDamage_Implementation(float damage) {
	entity_health = (entity_health.GetValue() - damage);
	
	if (entity_health.GetValue() <= 0) {
		entity_health = 0;
		//TODO: die
	}
}

bool APowerEntity::ServerPowerTakeDamage_Validate(float damage) {
	return true;
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