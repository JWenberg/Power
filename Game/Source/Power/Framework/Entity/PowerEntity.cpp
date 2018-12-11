 

#include "PowerEntity.h"
#include "UnrealNetwork.h"
#include "PowerEntityAttributeSet.h"
#include "Classes/Components/DecalComponent.h"


// Sets default values
APowerEntity::APowerEntity()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    this->TargetCircle = CreateDefaultSubobject<UDecalComponent>(TEXT("TargetCircle"));
    this->TargetCircle->DecalSize = FVector(64.f, 64.f, 64.f);
    this->TargetCircle->SetupAttachment(RootComponent);

    static ConstructorHelpers::FObjectFinder<UMaterial> DecalMaterial(TEXT("Material'/Game/Power/Materials/TargetDecal/M_DecalTarget.M_DecalTarget'"));
    if (DecalMaterial.Succeeded()) {
        this->TargetCircle->SetMaterial(0, DecalMaterial.Object);
    }

    this->TargetCircle->SetRelativeRotation(FQuat(180.f, 90.f, 180.f, 0.0f));

    //Ability system
    this->AbilitySystem = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystem"));
    this->AttributeSet = CreateDefaultSubobject<UPowerEntityAttributeSet>(TEXT("PowerEntityAttributeSet"));

	//Default Attributes
	this->Mana = 1000;
	this->MaxMana = 1000;
	this->Level = 0;
	this->Name = "Jansen";
	this->Guild = "IsInAGuild";

	//Nameplate Component
	static ConstructorHelpers::FClassFinder<UNameplateController> NameplateReference(TEXT("/Game/Power/UI/NamePlates/BP_NameplateController"));
	if (NameplateReference.Succeeded()) {
		this->NameplateController = static_cast<UNameplateController*>(CreateDefaultSubobject(TEXT("NameplateController"), UNameplateController::StaticClass(), NameplateReference.Class.Get(), true, false, false));
		this->NameplateController->SetupAttachment(RootComponent);
		this->NameplateController->SetRelativeLocation(FVector(0, 0, 120));
		this->NameplateController->UpdateNameplate();
	}
}

// Called when the game starts or when spawned
void APowerEntity::BeginPlay()
{
	Super::BeginPlay();
    this->Mana = 1000;
    this->Level = 1;
    this->TargetCircle->SetRelativeLocation(FVector(0.f, 0.f, -110.f));
    this->TargetCircle->SetVisibility(false);
	UpdateHUD();
}

// Called every frame
void APowerEntity::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APowerEntity::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void APowerEntity::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const {

    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    // Replicate to everyone
    DOREPLIFETIME(APowerEntity, Mana);
    DOREPLIFETIME(APowerEntity, Level);
    DOREPLIFETIME(APowerEntity, TargetEntity);
	DOREPLIFETIME(APowerEntity, Name);
}

void APowerEntity::GiveAbility(TSubclassOf<UGameplayAbility> Ability)
{
    if (AbilitySystem) {
        if (HasAuthority() && Ability) {
            AbilitySystem->GiveAbility(FGameplayAbilitySpec(Ability));
        }

        AbilitySystem->InitAbilityActorInfo(this, this);
    }
}

int APowerEntity::GetHealth()
{
    return AttributeSet->Health.GetCurrentValue();
}

int APowerEntity::GetMaxHealth()
{
    return AttributeSet->MaxHealth.GetCurrentValue();
}

void APowerEntity::SetTargetAndHandleCircle(APowerEntity * EntityToTarget)
{
    if (this->TargetEntity) {
        this->TargetEntity->TargetCircle->SetVisibility(false);
    }

    this->ChangeTarget(EntityToTarget);

    if (EntityToTarget) {
        EntityToTarget->TargetCircle->SetVisibility(true);
    }
	UpdateHUD();
}

void APowerEntity::ChangeTarget(APowerEntity* NewTarget)
{
    if (Role < ROLE_Authority) {
        ServerChangeTarget(NewTarget);
    }

    this->TargetEntity = NewTarget;
}

void APowerEntity::ServerChangeTarget_Implementation(APowerEntity* NewTarget)
{
    ChangeTarget(NewTarget);
}

bool APowerEntity::ServerChangeTarget_Validate(APowerEntity* NewTarget)
{
    return true;
}


void APowerEntity::CastAbilityOnTarget(TSubclassOf<UGameplayAbility> AbilityToCast, FGameplayTag EventTag) {
    if (TargetEntity == nullptr) {
        return;
    }
    
    if (Role < ROLE_Authority) {
        ServerCastAbilityOnTarget(AbilityToCast, EventTag);
    }
        
    AbilitySystem->TryActivateAbilityByClass(AbilityToCast);
    FGameplayEventData Payload;
    Payload.Target = TargetEntity;
    AbilitySystem->HandleGameplayEvent(EventTag, &Payload);
}

void APowerEntity::ServerCastAbilityOnTarget_Implementation(TSubclassOf<UGameplayAbility> AbilityToCast, FGameplayTag EventTag)
{
    CastAbilityOnTarget(AbilityToCast, EventTag);
}

bool APowerEntity::ServerCastAbilityOnTarget_Validate(TSubclassOf<UGameplayAbility> AbilityToCast, FGameplayTag EventTag)
{
    return true;
}