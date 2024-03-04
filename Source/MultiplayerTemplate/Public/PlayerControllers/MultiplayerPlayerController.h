// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MultiplayerPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERTEMPLATE_API AMultiplayerPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AMultiplayerPlayerController();
	friend class ALobbyGameMode;
	
	UFUNCTION(Server, Reliable)
	void S_SetSavedSkeletalMesh(USkeletalMesh* NewSkeletalMesh);

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
private:
	UPROPERTY(ReplicatedUsing = OnRep_SavedSkeletalMeshUpdated)
	USkeletalMesh* SavedSkeletalMesh;

	UFUNCTION()
	void OnRep_SavedSkeletalMeshUpdated();
	
	UFUNCTION(Server, Reliable)
	void S_LoadSavedDataToLobbyGM();
};
