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

protected:

private:
	class USkeletalMesh* SavedSkeletalMeshSelection;



};
