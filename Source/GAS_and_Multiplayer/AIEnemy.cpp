// Fill out your copyright notice in the Description page of Project Settings.


#include "AIEnemy.h"
#include "GASGameState.h"
#include "AbilitySystemComponent.h"
#include "ShootingWeapon.h"
#include "GASAttributeSet.h"
#include "GameplayEffect.h"
#include "Net/UnrealNetwork.h"


// Sets default values
AAIEnemy::AAIEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HealthBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBarWidget"));
	HealthBarWidget->SetupAttachment(GetMesh());
	
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));

	SetReplicates(true);

	MaxHealth = CurrentHealth = 0.f ;

	IsMoving = IsDead = false ;

	Tags.Add(TEXT("Enemy"));
}

// Later I will add some multiplayer features to the game .

void AAIEnemy::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AAIEnemy, IsMoving);
	DOREPLIFETIME(AAIEnemy, IsDead);
}

// Called when the game starts or when spawned
void AAIEnemy::BeginPlay()
{
	Super::BeginPlay();

	GameState = GetWorld()->GetGameState<AGASGameState>() ;

	SpawnShootingWeapon();

	AttributeSet = AbilitySystemComponent->GetSet<UGASAttributeSet>();
	AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(ShootingAbility, 0, 1, this));

	// We will get the health value from the attribute set

	MaxHealth = CurrentHealth = AbilitySystemComponent->GetNumericAttribute(UGASAttributeSet::GetHealthAttribute());
}

// Called every frame
void AAIEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// We will update the current health value from the attribute set.

	CurrentHealth = AbilitySystemComponent->GetNumericAttribute(UGASAttributeSet::GetHealthAttribute());

	if(IsDead)
	{
		HealthBarWidget->SetVisibility(false);
		DetachFromControllerPendingDestroy();
		SetActorTickEnabled(false);
		return ;
	}
}

void AAIEnemy::SpawnShootingWeapon()
{
	if (TheShootingWeapon)
	{
		ShootingWeapon = GetWorld()->SpawnActor<AShootingWeapon>(TheShootingWeapon);
		ShootingWeapon->SetOwner(this);
		ShootingWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("EquipSocket"));
		ShootingWeapon->SetReplicates(true);
	}
}

void AAIEnemy::ActivateShootingAbility()
{
	AbilitySystemComponent->TryActivateAbilityByClass(ShootingAbility);
}

float AAIEnemy::GetAIEnemyHealthPercentage() const
{
	return CurrentHealth / MaxHealth ;
}

bool AAIEnemy::GetIsMoving() const 
{
	return IsMoving ;
}

bool AAIEnemy::GetIsDead()
{
	if(GameState == nullptr || (GameState->RemainingTime > (GameState->MaxTime - 3)))
	{
		return false ;
	}
	
	if (GetAIEnemyHealthPercentage() <= 0)
	{
		IsDead= true ;
	}

	return IsDead ;
}

