// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayerCharacterController.generated.h"

/**
 * 
 */
UCLASS()
class GAS_AND_MULTIPLAYER_API APlayerCharacterController : public APlayerController
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> TheGameplayWidget ;
	class UUserWidget * GameplayWidget ;

protected:
	virtual void BeginPlay() override ;
};
