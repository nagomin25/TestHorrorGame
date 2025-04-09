// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TestHorrorGameGameMode.generated.h"

UCLASS(minimalapi)
class ATestHorrorGameGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ATestHorrorGameGameMode();
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GameMode")
	TSubclassOf<AActor> DefaultPawnClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GameMode")
	TSubclassOf<AActor> DefaultHUDClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GameMode")
	TSubclassOf<AActor> DefaultPlayerStateClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GameMode")
	TSubclassOf<AActor> DefaultGameStateClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GameMode")
	TSubclassOf<AActor> DefaultPlayerControllerClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GameMode")
	TSubclassOf<AActor> DefaultSpectatorClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GameMode")
	TSubclassOf<AActor> DefaultGameModeClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GameMode")
	TSubclassOf<AActor> DefaultGameSessionClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GameMode")
	TSubclassOf<AActor> DefaultReplaySpectatorClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GameMode")
	TSubclassOf<AActor> DefaultReplayGameModeClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GameMode")
	TSubclassOf<AActor> DefaultReplayGameSessionClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GameMode")
	TSubclassOf<AActor> DefaultReplaySpectatorPawnClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GameMode")
	TSubclassOf<AActor> DefaultReplayPlayerStateClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GameMode")
	TSubclassOf<AActor> DefaultReplayPlayerControllerClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GameMode")
	TSubclassOf<AActor> DefaultReplayHUDClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GameMode")
	TSubclassOf<AActor> DefaultReplaySpectatorHUDClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GameMode")
	TSubclassOf<AActor> DefaultReplaySpectatorControllerClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GameMode")
	TSubclassOf<AActor> DefaultReplaySpectatorPawnClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GameMode")
	TSubclassOf<AActor> DefaultReplaySpectatorPlayerStateClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GameMode")
	TSubclassOf<AActor> DefaultReplaySpectatorPlayerControllerClass;
	ATestHorrorGameGameMode();
};



