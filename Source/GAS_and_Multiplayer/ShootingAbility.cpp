// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootingAbility.h"
#include "GASAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "ShootingCostGameplayEffect.h"
#include "ShootingCooldownGameplayEffect.h"
#include "PlayerCharacter.h"
#include "AIEnemy.h"
#include "ShootingWeapon.h"
// #include "GameplayEffect.h"
// #include "GameplayEffectTypes.h"



UShootingAbility::UShootingAbility()
{
    CostGameplayEffectClass = UShootingCostGameplayEffect::StaticClass();
}


void UShootingAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    CommitAbility(Handle, ActorInfo, ActivationInfo);

    AbilitySystemComponent = GetAbilitySystemComponentFromActorInfo();

    FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
    EffectContext.AddSourceObject(this);

    if (ActorInfo->AvatarActor.IsValid())
    {
        // Check if the AvatarActor is a PlayerCharacter or AIEnemy and call the Shoot method and apply the cooldown effect accordingly.

        if (ActorInfo->AvatarActor.Get()->ActorHasTag("Player"))
        {
            APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(ActorInfo->AvatarActor.Get());
            PlayerCharacter->ShootingWeapon->Shoot();

            FGameplayEffectSpecHandle EffectSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(PlayerCharacter->ShootingCooldownEffect , 1.0f, EffectContext);
            if (EffectSpecHandle.IsValid())
            {
                AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
            }
        }
        else
        {
            AAIEnemy* AIEnemy = Cast<AAIEnemy>(ActorInfo->AvatarActor.Get());
            AIEnemy->ShootingWeapon->Shoot();

            FGameplayEffectSpecHandle EffectSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(AIEnemy->ShootingCooldownEffect , 1.0f, EffectContext);
            if (EffectSpecHandle.IsValid())
            {
                AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
            }
        }
    }

    EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
