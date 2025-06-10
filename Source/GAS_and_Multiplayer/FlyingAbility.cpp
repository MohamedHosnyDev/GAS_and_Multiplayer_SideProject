// Fill out your copyright notice in the Description page of Project Settings.


#include "FlyingAbility.h"
#include "AbilitySystemComponent.h"
#include "PlayerCharacter.h"



void UFlyingAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    CommitAbility(Handle, ActorInfo, ActivationInfo);

    if (ActorInfo->AvatarActor.IsValid())
    {
        APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(ActorInfo->AvatarActor.Get());
        AbilitySystemComponent = GetAbilitySystemComponentFromActorInfo();
        FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
        EffectContext.AddSourceObject(this);
        FGameplayEffectSpecHandle EffectSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(PlayerCharacter->FlyingCostEffect, 1.0f, EffectContext);
        if (EffectSpecHandle.IsValid())
        {
            AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
        }

        PlayerCharacter->Fly();
    }
    
}
