// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "MultiplayerSaveGameData.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERTEMPLATE_API UMultiplayerSaveGameData : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere)
	FName SavedSelectionID;

protected:

private:

	
};
