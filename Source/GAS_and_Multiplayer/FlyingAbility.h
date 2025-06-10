// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "FlyingAbility.generated.h"

/**
 * 
 */
UCLASS()
class GAS_AND_MULTIPLAYER_API UFlyingAbility : public UGameplayAbility
{
	GENERATED_BODY()

	class UAbilitySystemComponent * AbilitySystemComponent ;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
};
