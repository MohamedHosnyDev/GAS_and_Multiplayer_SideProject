// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTaskAttackPlayer.generated.h"

/**
 * 
 */
UCLASS()
class GAS_AND_MULTIPLAYER_API UBTTaskAttackPlayer : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
	UBTTaskAttackPlayer();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
