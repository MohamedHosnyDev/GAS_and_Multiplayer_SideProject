// Fill out your copyright notice in the Description page of Project Settings.


#include "GASGameState.h"
#include "TimerManager.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/DirectionalLight.h"
#include "Kismet/GameplayStatics.h"



AGASGameState::AGASGameState()
{
    PrimaryActorTick.bCanEverTick = true;

    RemainingTime = 0 ;
}


void AGASGameState::BeginPlay()
{
    Super::BeginPlay();

    RemainingTime = MaxTime ;
    GetWorldTimerManager().SetTimer(CountDownHandler,this,&AGASGameState::DecreaseRemainingTime,1,true);
    
    PlayGameStateWidget();

}

void AGASGameState::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if(GetRemainingTimeInMinutes() == 2 && GetRemainingTimeInSeconds() == 0)
    {
        TurnDayToNight();

        SetActorTickEnabled(false);
    }
}

void AGASGameState::PlayGameStateWidget()
{
    GameStateWidget = CreateWidget(GetWorld(),TheGameStateWidget);
    GameStateWidget -> AddToViewport();
}

int AGASGameState::GetRemainingTimeInSeconds()
{
    return RemainingTime % 60 ;
}

int AGASGameState::GetRemainingTimeInMinutes()
{
    int RemainingTimeInMinutes = (RemainingTime - (RemainingTime % 60)) / 60 ;

    return RemainingTimeInMinutes ;
}

void AGASGameState::DecreaseRemainingTime()
{
    if(RemainingTime == 0) return ;
    
    RemainingTime -- ;
}

int AGASGameState::GetNumberOfAlivePlayers()
{
    TArray<AActor*> Players;
    UGameplayStatics::GetAllActorsWithTag(GetWorld(),TEXT("Player"),Players);
    return Players.Num();
}

void AGASGameState::TurnDayToNight()
{
    TArray<AActor*> DirectionalLightActorsArray;
    UGameplayStatics::GetAllActorsWithTag(GetWorld(),TEXT("DirectionalLight"),DirectionalLightActorsArray);
    ADirectionalLight * DirectionalLightActor = Cast<ADirectionalLight>(DirectionalLightActorsArray[0]);
    if(DirectionalLightActor)
    {
        DirectionalLightActor->SetActorRotation(FRotator(0,0,0));
    }

    if(DayToNightSound)
    {
        UGameplayStatics::PlaySound2D(GetWorld(),DayToNightSound);
    }
}

