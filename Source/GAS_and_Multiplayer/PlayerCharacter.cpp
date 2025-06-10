// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "GASGameState.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/WidgetComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "AbilitySystemComponent.h"
#include "ShootingWeapon.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "GASAttributeSet.h"
#include "GameplayEffect.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(RootComponent);
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);
	
	HealthAndFlyingEnergyWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthAndFlyingEnergyWidget"));
	HealthAndFlyingEnergyWidget->SetupAttachment(GetMesh());

	AimingSignWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("AimingSignWidget"));

	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));

	SetReplicates(true);

	MaxHealth = CurrentHealth = MaxFlyingEnergy = CurrentFlyingEnergy = 0.f ;
	MovingRotationYawValue = 0.0f ;

	Tags.Add(TEXT("Player"));

	LatestDistanceBetweenPlayerAndHitPoint = 0.0f ;
	CameraIsMoved = IsMoving = IsFlying = IsDead = false;
	CanIncreaseFlyingEnergy = true ;

}

// Later I will add some multiplayer features to the game .

void APlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APlayerCharacter, IsDead);
	DOREPLIFETIME(APlayerCharacter, IsFlying);
	DOREPLIFETIME(APlayerCharacter, MovingRotationYawValue);
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	GameState = GetWorld()->GetGameState<AGASGameState>();

	SpawnShootingWeapon();

	AttributeSet = AbilitySystemComponent->GetSet<UGASAttributeSet>();
	AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(ShootingAbility, 1, 1, this));
	AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(FlyingAbility, 1, 1, this));

	// We will get the health and flying energy values from the attribute set .

	MaxHealth = CurrentHealth = AbilitySystemComponent->GetNumericAttribute(UGASAttributeSet::GetHealthAttribute());
	MaxFlyingEnergy = CurrentFlyingEnergy = AbilitySystemComponent->GetNumericAttribute(UGASAttributeSet::GetFlyingEnergyAttribute());

	SpringArmFirstLocationZValue = SpringArmComponent->GetRelativeLocation().Z;
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(IsDead)
	{
		HealthAndFlyingEnergyWidget -> SetVisibility(false);
		AimingSignWidget -> SetVisibility(false);
		SetActorTickEnabled(false);
		return ;
	}

	MoveAimingSignAndCamera();
	
	// We will update the current health and flying energy values from the attribute set every tick .

	CurrentHealth = AbilitySystemComponent->GetNumericAttribute(UGASAttributeSet::GetHealthAttribute());
	CurrentFlyingEnergy = AbilitySystemComponent->GetNumericAttribute(UGASAttributeSet::GetFlyingEnergyAttribute());

	/* If tif the player ain`t flying and the current flying energy is less than the maximum flying energy -
	We will activate the increase flying energy effect after 2 seconds of the player being on the ground . */

	if(CurrentFlyingEnergy < MaxFlyingEnergy && !GetMovementComponent()->IsFalling() && CanIncreaseFlyingEnergy)
	{
		ActivateIncreaseFlyingEnergyEffect();
	}

}

void APlayerCharacter::SpawnShootingWeapon()
{
	if (TheShootingWeapon)
	{
		ShootingWeapon = GetWorld()->SpawnActor<AShootingWeapon>(TheShootingWeapon);
		ShootingWeapon->SetOwner(this);
		ShootingWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
		ShootingWeapon->SetReplicates(true);
	}
}

void APlayerCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(InputMappingContext, 0);
		}
	}
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("RotatePlayer"),this,&APawn::AddControllerYawInput);

	if(UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveForwardOrBackwardAction, ETriggerEvent::Triggered, this, &APlayerCharacter::MoveForwardOrBackward);

		EnhancedInputComponent->BindAction(MoveRightOrLeftAction, ETriggerEvent::Triggered, this, &APlayerCharacter::MoveRightOrLeft);

		EnhancedInputComponent->BindAction(ShootingAbilityAction, ETriggerEvent::Triggered, this, &APlayerCharacter::ActivateShootingAbility);

		EnhancedInputComponent->BindAction(FlyingAbilityAction, ETriggerEvent::Triggered, this, &APlayerCharacter::ActivateFlyingAbility);

		EnhancedInputComponent->BindAction(StopFlyingAction, ETriggerEvent::Triggered, this, &APlayerCharacter::StopFlyingByInputAction);
	}
}

void APlayerCharacter::MoveForwardOrBackward(const FInputActionValue& Value)
{
	if(IsDead || IsFlying) return ;

	// We will set the MovingRotationYawValue depending on the direction of the player movement to use it in the animation blueprint .

	if(Value.Get<float>() > 0.0f)
	{
		MovingRotationYawValue = 0.0f;
	}
	else
	{
		MovingRotationYawValue = 180.0f;
	}

	AddMovementInput(GetMesh()->GetRightVector() * Value.Get<float>());
}

void APlayerCharacter::MoveRightOrLeft(const FInputActionValue& Value)
{
	if(IsDead || IsFlying) return ;

	// We will set the MovingRotationYawValue depending on the direction of the player movement to use it in the animation blueprint .

	if(Value.Get<float>() > 0.0f)
	{
		MovingRotationYawValue = 90.0f;
	}
	else
	{
		MovingRotationYawValue = -90.0f;
	}

	AddMovementInput((GetMesh()->GetForwardVector()* -1) * Value.Get<float>());
}

void APlayerCharacter::MoveAimingSignAndCamera()
{	
	if(IsDead) return;

	/* We will use the mouse cursor hit actor to move the aiming sign and camera spring arm -
	Depending on the distance between the player and the hit point . */

	APlayerController * PlayerController = Cast<APlayerController>(GetController());
	if(!PlayerController) return;
	
	PlayerController->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility,false,MouseCursorHitActor);

	if(!MouseCursorHitActor.GetActor()) return;
	AimingSignWidget->SetWorldLocation(MouseCursorHitActor.ImpactPoint);

	float DistanceBetweenPlayerAndHitPoint = FVector::Dist(GetActorLocation() , MouseCursorHitActor.ImpactPoint);

	if(DistanceBetweenPlayerAndHitPoint > 800 && DistanceBetweenPlayerAndHitPoint <= 1200 && DistanceBetweenPlayerAndHitPoint > LatestDistanceBetweenPlayerAndHitPoint)
	{
		FVector SpringArmLocation = SpringArmComponent->GetRelativeLocation();
		float SpringArmLocationZValue = SpringArmFirstLocationZValue + ((DistanceBetweenPlayerAndHitPoint - 800) / 2) ;
		SpringArmComponent->SetRelativeLocation(FVector(SpringArmLocation.X, SpringArmLocation.Y,SpringArmLocationZValue));
		LatestDistanceBetweenPlayerAndHitPoint = DistanceBetweenPlayerAndHitPoint ;
		CameraIsMoved = true;
	}
	else if(DistanceBetweenPlayerAndHitPoint <= 800 && CameraIsMoved)
	{
		FVector SpringArmLocation = SpringArmComponent->GetRelativeLocation();
		SpringArmComponent->SetRelativeLocation(FVector(SpringArmLocation.X, SpringArmLocation.Y, SpringArmFirstLocationZValue));
		LatestDistanceBetweenPlayerAndHitPoint = 0 ;
		CameraIsMoved = false;
	}
	
}

void APlayerCharacter::Fly()
{	
	if(!CanIncreaseFlyingEnergy || IsDead) return ;


	if(!GetCharacterMovement()->IsFalling() && !IsFlying)
	{
		// If the player is not flying and is on the ground - we will start flying and spawn the hang glider actor if it exists .

		if(TheHangGlider)
		{
			HangGlider = GetWorld()->SpawnActor<AActor>(TheHangGlider);
			HangGlider->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("HangGliderSocket"));
			HangGlider->SetReplicates(true);

			if(HangGliderSound)
			{
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), HangGliderSound, GetActorLocation());
			}
		}

		SetActorLocation(GetActorLocation() + FVector(0, 0, 700));
		IsFlying = true;
		Tags.Add("Flying");
		return ;
	}
	else if(GetCharacterMovement()->IsFalling() && CurrentFlyingEnergy > 0)
	{
		/* If the player is flying and has enough flying energy -
		We will continue flying and apply the gravity scale new value to the character movement component . */

		GetCharacterMovement()->GravityScale = 0.1f;
		SetActorLocation(GetActorLocation() + (GetActorForwardVector() * 20));
	}
	else if (CurrentFlyingEnergy <= 0  && IsFlying)
	{
		// If the player is flying and has no enough flying energy He will stop flying .

		StopFlying();
	}
	else 
	{
		// If the player is not flying and is on the ground - we will stop flying if he is flying .

		StopFlying();
	}
}

void APlayerCharacter::StopFlying()
{
	/* This method will destroy the hang glider actor if it exists, set the flying state to false -
	And reset the gravity scale of the character movement component. */

	if(HangGlider && IsFlying)
	{
		HangGlider->Destroy();
		IsFlying = false;
		GetCharacterMovement()->GravityScale = 1.0f;
		Tags.Remove("Flying");
	}

	CanIncreaseFlyingEnergy = false ;
	GetWorldTimerManager().SetTimer(IncreaseFlyingEnergyTimerHandle, this, &APlayerCharacter::SetCanIncreaseFlyingEnergyToTrue, 2.0f, true);
}

void APlayerCharacter::StopFlyingByInputAction(const FInputActionValue& Value)
{
	if(IsFlying)
	{
		StopFlying();
	}
}

void APlayerCharacter::ActivateIncreaseFlyingEnergyEffect()
{		
	UE_LOG(LogTemp, Warning, TEXT("Increase fly effect"));
	
	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);
	FGameplayEffectSpecHandle EffectSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(IncreaseFlyingEnergyEffect, 1.0f, EffectContext);
	if (EffectSpecHandle.IsValid())
	{
		AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
	}
}

void APlayerCharacter::ActivateShootingAbility(const FInputActionValue& Value)
{
	if(IsDead || IsFlying) return ;

	AbilitySystemComponent->TryActivateAbilityByClass(ShootingAbility);
}


void APlayerCharacter::ActivateFlyingAbility(const FInputActionValue& Value)
{
	if(IsDead) return ;

	AbilitySystemComponent->TryActivateAbilityByClass(FlyingAbility);
}

float APlayerCharacter::GetHealthPercentage() const
{
	return CurrentHealth / MaxHealth ;
}

float APlayerCharacter::GetFlyingEnergyPercentage() const
{
	return CurrentFlyingEnergy / MaxFlyingEnergy ;
}

float APlayerCharacter::GetCurrentAmmu() const
{
	float CurrentAmmu = AbilitySystemComponent->GetNumericAttribute(UGASAttributeSet::GetAmmuAttribute());
	return CurrentAmmu ;
}

float APlayerCharacter::GetMovingRotationYawValue() const
{
	return MovingRotationYawValue;
}

bool APlayerCharacter::GetIsFlying() const
{
	return IsFlying;
}

void APlayerCharacter::SetCanIncreaseFlyingEnergyToTrue()
{
	CanIncreaseFlyingEnergy = true ;
}

bool APlayerCharacter::GetIsDead()
{
	if(GameState == nullptr || (GameState->RemainingTime > (GameState->MaxTime - 3)))
	{
		return false ;
	}
	
	if (GetHealthPercentage() <= 0)
	{
		Tags.Add(TEXT("Dead"));
		IsDead = true;
	}
	
	return IsDead;
}
