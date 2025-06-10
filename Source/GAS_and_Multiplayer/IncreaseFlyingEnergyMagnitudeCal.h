// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "IncreaseFlyingEnergyMagnitudeCal.generated.h"

/**
 * 
 */
UCLASS()
class GAS_AND_MULTIPLAYER_API UIncreaseFlyingEnergyMagnitudeCal : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()
	
	FGameplayEffectAttributeCaptureDefinition FlyingEnergyDef;

	UIncreaseFlyingEnergyMagnitudeCal();

	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override ;
	
};
