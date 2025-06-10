// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "TimerManager.h"
#include "GASGameState.generated.h"

/**
 * 
 */
UCLASS()
class GAS_AND_MULTIPLAYER_API AGASGameState : public AGameStateBase
{
	GENERATED_BODY()
	
	public :
		UPROPERTY(EditAnywhere)
		int MaxTime = 240 ;
		int RemainingTime ;
		struct FTimerHandle CountDownHandler ;

		UPROPERTY(EditAnywhere)
		TSubclassOf<class UUserWidget> TheGameStateWidget ;
		class UUserWidget * GameStateWidget ;
		UPROPERTY(EditAnywhere)
		class USoundBase * DayToNightSound ;

		// int NumOfAlivePlayers ;

		AGASGameState();

		virtual void Tick(float DeltaTime) override ;

		void PlayGameStateWidget();

		UFUNCTION(BlueprintPure)
		int GetRemainingTimeInSeconds();

		UFUNCTION(BlueprintPure)
		int GetRemainingTimeInMinutes();

		UFUNCTION(BlueprintCallable)
		void DecreaseRemainingTime();

		UFUNCTION(BlueprintPure)
		int GetNumberOfAlivePlayers();

		void TurnDayToNight();

	protected :

		virtual void BeginPlay() override ;


};
