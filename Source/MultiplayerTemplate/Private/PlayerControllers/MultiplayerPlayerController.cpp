// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerControllers/MultiplayerPlayerController.h"
#include "Characters/BasicCharacter.h"
#include "GameModes/LobbyGameMode.h"
#include "SaveGame/MultiplayerSaveGameData.h"

#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"

AMultiplayerPlayerController::AMultiplayerPlayerController()
{
    SetReplicates(true);
}

void AMultiplayerPlayerController::S_SetSavedSkeletalMesh_Implementation(USkeletalMesh *NewSkeletalMesh)
{
    SavedSkeletalMesh = NewSkeletalMesh;
}

void AMultiplayerPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(AMultiplayerPlayerController, SavedSkeletalMesh);
}

void AMultiplayerPlayerController::OnRep_SavedSkeletalMeshUpdated()
{
   Cast<ABasicCharacter>(GetPawn())->LoadCharacterSelection(SavedSkeletalMesh);
}

void AMultiplayerPlayerController::S_LoadSavedDataToLobbyGM_Implementation()
{
    AGameModeBase* GM = UGameplayStatics::GetGameMode(this);
    if(GM)
    {
        ALobbyGameMode* GameMode =  Cast<ALobbyGameMode>(GM);
        if(GameMode)
        {

            UMultiplayerSaveGameData* SaveData = Cast<UMultiplayerSaveGameData>(UGameplayStatics::CreateSaveGameObject(UMultiplayerSaveGameData::StaticClass()));
            SaveData = Cast<UMultiplayerSaveGameData>(UGameplayStatics::LoadGameFromSlot("CharacterSelection", 0));
            GameMode->SaveNewPlayerSaveDataToGameInstance(this, SaveData->SavedSelectionID);
        }
    }
    
    
}