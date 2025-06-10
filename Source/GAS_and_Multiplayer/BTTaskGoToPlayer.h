// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTaskGoToPlayer.generated.h"

/**
 * 
 */
UCLASS()
class GAS_AND_MULTIPLAYER_API UBTTaskGoToPlayer : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
	class AAIController * AIController ;
	AActor * PlayerActor ;
	FVector PlayerLocation ;
	class AAIEnemy * Enemy ;

	float DeltaSeconds ;
	FVector EnemyOldLocation ;
	FVector EnemyNewLocation ;
	FRotator EnemyNewRotation ;
	FVector OffsetVectorToPlayer ;

	public:
	UBTTaskGoToPlayer();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)override;
};
