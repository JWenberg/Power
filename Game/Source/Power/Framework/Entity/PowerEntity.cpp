 

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
	this->Health = 100;
	this->MaxHealth = 100;
	this->Mana = 1000;
	this->MaxMana = 1000;
	this->Level = 0;
	this->Name = "DefaultName";
	this->Guild = "";

	//Nameplate Component
	//this->NameplateController = CreateDefaultSubobject<UNameplateController>(TEXT("Nameplate"));
	/*static ConstructorHelpers::FObjectFinder<UNameplateController> NameplateReference(TEXT("Blueprint'/Game/Power/UI/NamePlates/BP_NameplateController.BP_NameplateController'"));
	if (NameplateReference.Succeeded()) {
		this->NameplateController = NameplateReference.Object;
		this->NameplateController->SetRelativeScale3D(FVector(0.3, 0.3, 0.3));
		this->NameplateController->SetRelativeLocation(FVector(0, 0, 130));
		this->NameplateController->bEditableWhenInherited = true;
	}
	*/
}

// Called when the game starts or when spawned
void APowerEntity::BeginPlay()
{
	Super::BeginPlay();
	this->Health = 100;
	this->MaxHealth = 100;
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
    DOREPLIFETIME(APowerEntity, Health);
	DOREPLIFETIME(APowerEntity, MaxHealth);
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


void APowerEntity::CastAbilityOnTarget(TSubclassOf<UGameplayAbility> AbilityToCast) {
    //if (Role < ROLE_Authority) {
        //ServerCastAbilityOnTarget();
    //}
        
    //AbilitySystem->TryActivateAbilityByClass(AbilityToCast);

    //TODO : 
    // Server check
    // Get target
    // Build target data struct
    // TryActivateAbility
    // Send Gameplay tag
}
