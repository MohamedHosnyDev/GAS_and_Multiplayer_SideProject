// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TimerManager.h"
#include "PlayerCharacter.generated.h"

class UInputMappingContext ;
class UInputAction ;
struct FInputActionValue;

UCLASS()
class GAS_AND_MULTIPLAYER_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	class AGASGameState * GameState;

	UPROPERTY(EditAnywhere)
	class USpringArmComponent * SpringArmComponent;
	UPROPERTY(EditAnywhere)
	class UCameraComponent * CameraComponent;
	UPROPERTY(EditAnywhere)
	class UWidgetComponent * HealthAndFlyingEnergyWidget;
	UPROPERTY(EditAnywhere)
	class UWidgetComponent * AimingSignWidget;
	UPROPERTY(EditAnywhere)
	class UAbilitySystemComponent * AbilitySystemComponent;

	FHitResult MouseCursorHitActor;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AShootingWeapon> TheShootingWeapon;
	class AShootingWeapon * ShootingWeapon;
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> TheHangGlider;
	AActor * HangGlider;

	UPROPERTY(EditAnywhere, Category = "Input Actions")
	UInputMappingContext * InputMappingContext ;
	UPROPERTY(EditDefaultsOnly, Category = "Input Actions")
	UInputAction * MoveForwardOrBackwardAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input Actions")
	UInputAction * MoveRightOrLeftAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input Actions")
	UInputAction * MoveCameraAndRotatePlayerAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input Actions")
	UInputAction * ShootingAbilityAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input Actions")
	UInputAction * FlyingAbilityAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input Actions")
	UInputAction * StopFlyingAction;

	UPROPERTY(EditAnywhere)
	USoundBase * HangGliderSound;

	UPROPERTY(EditAnywhere)
	const class UGASAttributeSet * AttributeSet ; 
	UPROPERTY(EditAnywhere, Category = "GAS")
	TSubclassOf<class UGameplayAbility> ShootingAbility;
	UPROPERTY(EditAnywhere, Category = "GAS")
	TSubclassOf<class UGameplayAbility> FlyingAbility;

	UPROPERTY(EditAnywhere, Category = "GAS")
	TSubclassOf<class UGameplayEffect> DamageEffect;
	UPROPERTY(EditAnywhere, Category = "GAS")
	TSubclassOf<class UGameplayEffect> FlyingCostEffect;
	UPROPERTY(EditAnywhere, Category = "GAS")
	TSubclassOf<class UGameplayEffect> IncreaseFlyingEnergyEffect;
	UPROPERTY(EditAnywhere, Category = "GAS")
	TSubclassOf<class UGameplayEffect> ShootingCooldownEffect;

	float MaxHealth;
	float CurrentHealth;
	float MaxFlyingEnergy;
	float CurrentFlyingEnergy;

	UPROPERTY(Replicated)
	float MovingRotationYawValue;

	struct FTimerHandle IncreaseFlyingEnergyTimerHandle;

	float SpringArmFirstLocationZValue;
	float LatestDistanceBetweenPlayerAndHitPoint;

	bool CameraIsMoved;
	bool IsMoving;
	
	UPROPERTY(Replicated)
	bool IsFlying;
	bool CanIncreaseFlyingEnergy;
	UPROPERTY(Replicated)
	bool IsDead;




protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Sets default values for this character's properties
	APlayerCharacter();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SpawnShootingWeapon();

	virtual void NotifyControllerChanged() override ;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MoveForwardOrBackward(const FInputActionValue& Value);
	void MoveRightOrLeft(const FInputActionValue& Value);
	void MoveAimingSignAndCamera();

	void Fly();
	void StopFlying();
	void StopFlyingByInputAction(const FInputActionValue& Value);
	void ActivateIncreaseFlyingEnergyEffect();

	void ActivateShootingAbility(const FInputActionValue& Value);
	void ActivateFlyingAbility(const FInputActionValue& Value);

	UFUNCTION(BlueprintPure)
	float GetHealthPercentage() const;
	UFUNCTION(BlueprintPure)
	float GetFlyingEnergyPercentage() const;
	UFUNCTION(BlueprintPure)
	float GetCurrentAmmo() const ;
	UFUNCTION(BlueprintPure)
	float GetMovingRotationYawValue() const;

	UFUNCTION(BlueprintPure)
	bool GetIsFlying() const ;
	void SetCanIncreaseFlyingEnergyToTrue();
	UFUNCTION(BlueprintPure)
	bool GetIsDead();

};
