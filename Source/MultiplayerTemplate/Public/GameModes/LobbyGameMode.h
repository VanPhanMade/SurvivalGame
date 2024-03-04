// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LobbyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERTEMPLATE_API ALobbyGameMode : public AGameModeBase
{
	GENERATED_BODY()

private:
	TArray<FName> PlayerSaves;
protected:
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;

public:
	virtual void SaveNewPlayerSaveDataToGameInstance(APlayerController* NewLoggedInPlayer, FName SaveDataID);
};
