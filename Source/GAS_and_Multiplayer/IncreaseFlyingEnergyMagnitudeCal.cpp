// Fill out your copyright notice in the Description page of Project Settings.


#include "IncreaseFlyingEnergyMagnitudeCal.h"
#include "GASAttributeSet.h"



UIncreaseFlyingEnergyMagnitudeCal::UIncreaseFlyingEnergyMagnitudeCal()
{
    FlyingEnergyDef = FGameplayEffectAttributeCaptureDefinition(UGASAttributeSet::GetFlyingEnergyAttribute(), EGameplayEffectAttributeCaptureSource::Source, false);
    RelevantAttributesToCapture.Add(FlyingEnergyDef);
}


float UIncreaseFlyingEnergyMagnitudeCal::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
    const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
    const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

    FAggregatorEvaluateParameters Parameters;
    Parameters.SourceTags = SourceTags;
    Parameters.TargetTags = TargetTags;

    float FlyingEnergyMagnitude = 0.0f;
    GetCapturedAttributeMagnitude(FlyingEnergyDef, Spec, Parameters, FlyingEnergyMagnitude);
    
    if(FlyingEnergyMagnitude < 100.0f)
    {
        // If the Flying Energy is less than 100 Which is the default value, We increase it by 0.5 .
        
        return 0.5f ;
    }

    return 0 ;
}
