// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootingCostGameplayEffect.h"
#include "GASAttributeSet.h"



UShootingCostGameplayEffect::UShootingCostGameplayEffect()
{
    FGameplayModifierInfo ModifierInfo;
    ModifierInfo.Attribute = UGASAttributeSet::GetAmmuAttribute();
    ModifierInfo.ModifierOp = EGameplayModOp::Additive;
    ModifierInfo.ModifierMagnitude = FScalableFloat(-1.0f); 
    Modifiers.Add(ModifierInfo);
}
