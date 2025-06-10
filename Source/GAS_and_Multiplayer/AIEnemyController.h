// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AIEnemyController.generated.h"

/**
 * 
 */
UCLASS()
class GAS_AND_MULTIPLAYER_API AAIEnemyController : public AAIController
{
	GENERATED_BODY()

public:
	class AAIEnemy * AIEnemy ;

	virtual void Tick(float DeltaTime) override;
	
protected:
	virtual void BeginPlay() override ;

};
