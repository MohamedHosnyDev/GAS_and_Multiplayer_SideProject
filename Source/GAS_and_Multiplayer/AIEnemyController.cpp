// Fill out your copyright notice in the Description page of Project Settings.


#include "AIEnemyController.h"
#include "AIEnemy.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"



void AAIEnemyController::BeginPlay()
{
    Super::BeginPlay();

    AIEnemy = Cast<AAIEnemy>(GetPawn());

    if(AIEnemy->BehaviorTree)
    {
        RunBehaviorTree(AIEnemy->BehaviorTree);
    }

}

void AAIEnemyController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if(AIEnemy->BehaviorTree)
    {
        GetBlackboardComponent()->SetValueAsBool("IsDead", AIEnemy->IsDead);
    }
}