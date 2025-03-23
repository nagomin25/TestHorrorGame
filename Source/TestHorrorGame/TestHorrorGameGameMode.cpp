// Copyright Epic Games, Inc. All Rights Reserved.

#include "TestHorrorGameGameMode.h"
#include "TestHorrorGameCharacter.h"
#include "UObject/ConstructorHelpers.h"

ATestHorrorGameGameMode::ATestHorrorGameGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
