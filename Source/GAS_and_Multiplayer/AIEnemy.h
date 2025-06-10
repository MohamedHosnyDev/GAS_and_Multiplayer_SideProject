// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/WidgetComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "AIEnemy.generated.h"

UCLASS()
class GAS_AND_MULTIPLAYER_API AAIEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	class AGASGameState * GameState ;

	UPROPERTY(EditAnywhere)
	UBehaviorTree * BehaviorTree ;
	UPROPERTY(EditAnywhere)
	UWidgetComponent * HealthBarWidget;
	UPROPERTY(EditAnywhere)
	class UAbilitySystemComponent * AbilitySystemComponent;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AShootingWeapon> TheShootingWeapon ;
	class AShootingWeapon * ShootingWeapon ;

	UPROPERTY(EditAnywhere)
	const class UGASAttributeSet * AttributeSet ;
	UPROPERTY(EditAnywhere, Category = "GAS")
	TSubclassOf<class UGameplayAbility> ShootingAbility;

	UPROPERTY(EditAnywhere, Category = "GAS")
	TSubclassOf<class UGameplayEffect> DamageEffect;
	UPROPERTY(EditAnywhere, Category = "GAS")
	TSubclassOf<class UGameplayEffect> ShootingCooldownEffect;

	float MaxHealth;
	float CurrentHealth;

	UPROPERTY(Replicated)
	bool IsMoving ;
	UPROPERTY(Replicated)
	bool IsDead ;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Sets default values for this character's properties
	AAIEnemy();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SpawnShootingWeapon();

	void ActivateShootingAbility();

	UFUNCTION(BlueprintPure)
	float GetAIEnemyHealthPercentage() const ;

	UFUNCTION(BlueprintPure)
	bool GetIsMoving() const ;

	UFUNCTION(BlueprintPure)
	bool GetIsDead();

};
