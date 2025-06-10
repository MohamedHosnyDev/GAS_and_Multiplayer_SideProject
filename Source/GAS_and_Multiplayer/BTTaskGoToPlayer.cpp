// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskGoToPlayer.h"
#include "AIController.h"
#include "AIEnemy.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"


UBTTaskGoToPlayer::UBTTaskGoToPlayer()
{
    NodeName = "Go To Player";
}

EBTNodeResult::Type UBTTaskGoToPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

    Super::ExecuteTask(OwnerComp, NodeMemory);

    // This task will make the AI enemy go to the player actor that is set in the blackboard component.

    AIController = OwnerComp.GetAIOwner();

    Enemy = Cast<AAIEnemy>(AIController->GetPawn());

    PlayerActor = Cast<AActor>(AIController->GetBlackboardComponent()->GetValueAsObject(TEXT("PlayerActor")));
    if(PlayerActor == nullptr)
    {
        return EBTNodeResult::Failed;
    }

    PlayerLocation = PlayerActor->GetActorLocation() ;
    
    OffsetVectorToPlayer = PlayerLocation - Enemy->GetActorLocation();
    
    EnemyNewRotation = FRotator(Enemy->GetActorRotation().Pitch , OffsetVectorToPlayer.Rotation().Yaw, Enemy->GetActorRotation().Roll);
    EnemyNewLocation = Enemy->GetActorLocation() + (Enemy->GetActorForwardVector() * 300 * Enemy->GetWorld()->DeltaTimeSeconds);
    
    Enemy->SetActorRotation(EnemyNewRotation);
    Enemy->SetActorLocation(EnemyNewLocation);

    return EBTNodeResult::Succeeded;
}