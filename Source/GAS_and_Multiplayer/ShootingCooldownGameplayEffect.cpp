// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootingCooldownGameplayEffect.h"
// #include "GameplayEffectComponents/AssetTagsGameplayEffectComponent.h"
#include "GameplayTagsManager.h"
// #include "GameplayEffectTypes.h"
#include "GASAttributeSet.h"
#include "Kismet/GameplayStatics.h"



UShootingCooldownGameplayEffect::UShootingCooldownGameplayEffect()
{   
    /* This Gameplay effect will be the base of another gameplay effects in the editor -
    So we set the duration magnitude depending of the character`s types (Player or AI) */ 

    FGameplayModifierInfo ModifierInfo;
    ModifierInfo.Attribute = UGASAttributeSet::GetAmmoAttribute();
    ModifierInfo.ModifierOp = EGameplayModOp::Additive;
    ModifierInfo.ModifierMagnitude = FScalableFloat(1.0f); 
    Modifiers.Add(ModifierInfo);
}
