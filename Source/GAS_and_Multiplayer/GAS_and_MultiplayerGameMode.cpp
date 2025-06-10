// Copyright Epic Games, Inc. All Rights Reserved.

#include "GAS_and_MultiplayerGameMode.h"
#include "GAS_and_MultiplayerCharacter.h"
#include "UObject/ConstructorHelpers.h"

AGAS_and_MultiplayerGameMode::AGAS_and_MultiplayerGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
