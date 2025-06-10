// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootingWeapon.h"
#include "Net/UnrealNetwork.h"
#include "PlayerCharacter.h"
#include "AIEnemy.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AShootingWeapon::AShootingWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	SetRootComponent(RootComp);

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(RootComp);

	ShootingComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ShootingComponent"));
	ShootingComponent->SetupAttachment(WeaponMesh);
	

	SetReplicates(true);

}

// Called every frame
void AShootingWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DrawAimingLine();
}

void AShootingWeapon::DrawAimingLine()
{
	LineBegin = ShootingComponent->GetComponentLocation();
	LineEnd = GetOwner()->GetActorLocation() + GetOwner()->GetActorRotation().Vector()*6000;
	GetWorld()->LineTraceSingleByChannel(HitActor, LineBegin, LineEnd, ECollisionChannel::ECC_Visibility);

	if(GetOwner()->ActorHasTag(TEXT("Enemy"))) return ;
	DrawDebugLine(GetWorld(), LineBegin, HitActor.ImpactPoint, FColor::Blue, false, 0.05);
}

void AShootingWeapon::Shoot()
{
	if(HitActor.GetActor() == nullptr) return ;

	// Choosing the way of dameging the target depends on the type of the owner of the gun and the target character

	if(HitActor.GetActor()->ActorHasTag(TEXT("Player")) && GetOwner()->ActorHasTag(TEXT("Player")))
	{
		APlayerCharacter * Player = Cast<APlayerCharacter>(HitActor.GetActor());
		
		FGameplayEffectContextHandle EffectContext = Player->AbilitySystemComponent->MakeEffectContext();
		EffectContext.AddSourceObject(this);
		FGameplayEffectSpecHandle EffectSpecHandle = Player->AbilitySystemComponent->MakeOutgoingSpec(Player->DamageEffect, 1, EffectContext);
		if(EffectSpecHandle.IsValid())
		{
			Player->AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
		}
	}

	else if(HitActor.GetActor()->ActorHasTag(TEXT("Enemy")) && GetOwner()->ActorHasTag(TEXT("Player")))
	{
		AAIEnemy * Enemy = Cast<AAIEnemy>(HitActor.GetActor());

		FGameplayEffectContextHandle EffectContext = Enemy->AbilitySystemComponent->MakeEffectContext();
		EffectContext.AddSourceObject(this);
		FGameplayEffectSpecHandle EffectSpecHandle = Enemy->AbilitySystemComponent->MakeOutgoingSpec(Enemy->DamageEffect , 1 , EffectContext);
		if(EffectSpecHandle.IsValid())
		{
			Enemy->AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
		}
	}

	else if (HitActor.GetActor()->ActorHasTag(TEXT("Player")) && GetOwner()->ActorHasTag(TEXT("Enemy")))
	{
		APlayerCharacter * Player = Cast<APlayerCharacter>(HitActor.GetActor());
		
		FGameplayEffectContextHandle EffectContext = Player->AbilitySystemComponent->MakeEffectContext();
		EffectContext.AddSourceObject(this);
		FGameplayEffectSpecHandle EffectSpecHandle = Player->AbilitySystemComponent->MakeOutgoingSpec(Player->DamageEffect, 1, EffectContext);
		if(EffectSpecHandle.IsValid())
		{
			Player->AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
		}
	}

	if(ShootingSound && ShootingParticle && ImpactParticle && HitActor.GetActor())
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ShootingSound, LineBegin);
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ShootingParticle, LineBegin, GetActorRotation());
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticle, HitActor.ImpactPoint, GetActorRotation());
	}

}