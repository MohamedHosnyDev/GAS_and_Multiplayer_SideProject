// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShootingWeapon.generated.h"

UCLASS()
class GAS_AND_MULTIPLAYER_API AShootingWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	UPROPERTY(EditAnywhere)
	USceneComponent * RootComp ;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent * WeaponMesh;

	UPROPERTY(EditAnywhere)
	USceneComponent * ShootingComponent ;

	// UPROPERTY(EditAnywhere)
	// TSubclassOf<class UGameplayEffect> DamageEffect;

	// UPROPERY(EditAnywhere, Category = "GAS")
	// TSubclassOf<class UGameplayEffect> ShootingCostEffect;
	// UPROPERY(EditAnywhere, Category = "GAS")
	// TSubclassOf<class UGameplayEffect> ShootingCooldownEffect;

	UPROPERTY(EditAnywhere , Category = "ShootingProperties")
	USoundBase * ShootingSound;

	UPROPERTY(EditAnywhere, Category = "ShootingProperties")
	UParticleSystem * ShootingParticle;
	UPROPERTY(EditAnywhere, Category = "ShootingProperties")
	UParticleSystem * ImpactParticle;

	// The line that will be drawn to show the aiming direction And will be used to trace the hit actor
	FVector LineBegin;
	FVector LineEnd;
	FHitResult HitActor ;

public:	
	// Sets default values for this actor's properties
	AShootingWeapon();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void DrawAimingLine();

	void Shoot();

};
