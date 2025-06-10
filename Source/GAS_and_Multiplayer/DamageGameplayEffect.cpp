// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageGameplayEffect.h"
#include "GASAttributeSet.h"



UDamageGameplayEffect::UDamageGameplayEffect()
{
    FGameplayModifierInfo ModifierInfo;
    ModifierInfo.Attribute = UGASAttributeSet::GetHealthAttribute();
    ModifierInfo.ModifierOp = EGameplayModOp::Additive;
    ModifierInfo.ModifierMagnitude = FScalableFloat(-10.0f) ;
    Modifiers.Add(ModifierInfo);
}