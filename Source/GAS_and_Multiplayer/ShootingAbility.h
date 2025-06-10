// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "ShootingAbility.generated.h"

/**
 * 
 */
UCLASS()
class GAS_AND_MULTIPLAYER_API UShootingAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	class UAbilitySystemComponent * AbilitySystemComponent ;

	UShootingAbility();
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

};
