// Fill out your copyright notice in the Description page of Project Settings.


#include "BTServiceReachToThePlayer.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "AIEnemy.h"
#include "Kismet/GameplayStatics.h"



UBTServiceReachToThePlayer::UBTServiceReachToThePlayer()
{
    NodeName = TEXT("LookForThePlayer");
}

void UBTServiceReachToThePlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    /* This service will check if the AI enemy is close enough to the player actor to either attack or move towards them -
    If the player ain't flying or dead. */

    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController) return;

    AAIEnemy* AIEnemy = Cast<AAIEnemy>(AIController->GetPawn());
    if (!AIEnemy) return;

    TArray<AActor*> PlayersArray;
    UGameplayStatics::GetAllActorsWithTag(AIEnemy->GetWorld(), TEXT("Player"), PlayersArray);

    if (PlayersArray.Num() == 0)
    {
        AIController->GetBlackboardComponent()->ClearValue(TEXT("PlayerActor"));
        AIController->GetBlackboardComponent()->SetValueAsBool(TEXT("PlayerLocationIsReached"),false);
        return;
    }

    float DistanceBetweenEnemyAndPlayer ;
    AActor * ClosestPlayer = UGameplayStatics::FindNearestActor(AIEnemy->GetActorLocation(), PlayersArray, DistanceBetweenEnemyAndPlayer);


    if ((DistanceBetweenEnemyAndPlayer <= AttackRange) && !ClosestPlayer->ActorHasTag("Flying") && !ClosestPlayer->ActorHasTag(TEXT("Dead")))
    {
        AIController->GetBlackboardComponent()->SetValueAsBool(TEXT("PlayerLocationIsReached"),true);
        AIEnemy->IsMoving = false ;
    }
    else if ((DistanceBetweenEnemyAndPlayer <= GoToPlayerRange) && !ClosestPlayer->ActorHasTag("Flying")  && !ClosestPlayer->ActorHasTag(TEXT("Dead")))
    {
        AIController->GetBlackboardComponent()->SetValueAsObject(TEXT("PlayerActor"), ClosestPlayer);
        AIController->GetBlackboardComponent()->SetValueAsBool(TEXT("PlayerLocationIsReached"),false);
        AIEnemy->IsMoving = true ;
    }
    else if (ClosestPlayer->ActorHasTag("Flying")  || !ClosestPlayer->ActorHasTag(TEXT("Dead")))
    {
        AIController->GetBlackboardComponent()->ClearValue(TEXT("PlayerActor"));
        AIController->GetBlackboardComponent()->SetValueAsBool(TEXT("PlayerLocationIsReached"),false);
        AIEnemy->IsMoving = false ;
    }
    else
    {
        AIController->GetBlackboardComponent()->ClearValue(TEXT("PlayerActor"));
        AIController->GetBlackboardComponent()->SetValueAsBool(TEXT("PlayerLocationIsReached"),false);
        AIEnemy->IsMoving = false ;
    }
}