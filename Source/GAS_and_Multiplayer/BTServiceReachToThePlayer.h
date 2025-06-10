// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTServiceReachToThePlayer.generated.h"

/**
 * 
 */
UCLASS()
class GAS_AND_MULTIPLAYER_API UBTServiceReachToThePlayer : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditInstanceOnly)
	float GoToPlayerRange = 800.0f ;

	UPROPERTY(EditInstanceOnly)
    float AttackRange = 300.0f ;

	UBTServiceReachToThePlayer();

	void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
