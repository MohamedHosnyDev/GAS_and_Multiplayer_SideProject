// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskAttackPlayer.h"
#include "AIController.h"
#include "AIEnemy.h"
#include "BehaviorTree/BlackboardComponent.h"



UBTTaskAttackPlayer::UBTTaskAttackPlayer()
{
    NodeName = TEXT("AttackThePlayer");
}

EBTNodeResult::Type UBTTaskAttackPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);
    
    AAIController* AIController = OwnerComp.GetAIOwner();

    AAIEnemy* Enemy = Cast<AAIEnemy>(AIController->GetPawn());
    
    AActor * PlayerActor = Cast<AActor>(AIController->GetBlackboardComponent()->GetValueAsObject(TEXT("PlayerActor")));

    if(PlayerActor -> ActorHasTag(TEXT("Flying"))) return EBTNodeResult::Failed ;

    FVector OffsetVectorToPlayer = PlayerActor->GetActorLocation() - Enemy->GetActorLocation();

    Enemy->SetActorRotation(OffsetVectorToPlayer.Rotation());
    Enemy->ActivateShootingAbility();

    return EBTNodeResult::Succeeded;
}