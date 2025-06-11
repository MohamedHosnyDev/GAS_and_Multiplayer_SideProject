// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "Net/UnrealNetwork.h"
#include "GASAttributeSet.generated.h"

/**
 * 
 */

#define ATTRIBUTE_ACCESSORS(ClassName,PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName,PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName) \


UCLASS()
class GAS_AND_MULTIPLAYER_API UGASAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes", Replicated)
	FGameplayAttributeData Health ;
	ATTRIBUTE_ACCESSORS(UGASAttributeSet, Health)

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes", Replicated)
	FGameplayAttributeData Ammo ;
	ATTRIBUTE_ACCESSORS(UGASAttributeSet, Ammo)

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes", Replicated)
	FGameplayAttributeData FlyingEnergy ;
	ATTRIBUTE_ACCESSORS(UGASAttributeSet, FlyingEnergy)

private:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override ;

};
