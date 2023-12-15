// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "MainLevelGameMode.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERTEMPLATE_API AMainLevelGameMode : public AGameMode
{
	GENERATED_BODY()

public: 
	virtual void PlayerEliminated(class ABasicCharacter* EliminatedCharacter, class APlayerController* EliminatedPlayerController, class APlayerController* AttackerController);
	virtual void RequestRespawn(class ACharacter* RespawningCharacter, class AController* RespawningController);
};
