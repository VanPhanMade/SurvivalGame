// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MultiplayerGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERTEMPLATE_API UMultiplayerGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
	virtual void LoadSelectedCharacter(APlayerController* ThisPlayer);

	UFUNCTION()
	virtual void RegisterCharacterSelection(USkeletalMesh* NewSkeletalMesh);

	virtual void SaveIDsForPlayers(TArray<FName> NewPlayerSaveIDs);

	virtual void LoadPlayerData(APlayerController* ThisPlayer, FName DataID);
	
	class USkeletalMesh* SavedSkeletalMeshSelection;
protected:

private:
	TArray<FName> PlayerSaveIDs;
};
