// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInstances/MultiplayerGameInstance.h"
#include "Characters/BasicCharacter.h"
#include "PlayerControllers/MultiplayerPlayerController.h"
#include "DataTables/CharacterPresetsDataTable.h"

void UMultiplayerGameInstance::LoadSelectedCharacter(APlayerController *ThisPlayer)
{
    Cast<AMultiplayerPlayerController>(ThisPlayer)->S_SetSavedSkeletalMesh(SavedSkeletalMeshSelection);
}

void UMultiplayerGameInstance::RegisterCharacterSelection(USkeletalMesh *NewSkeletalMesh)
{
    SavedSkeletalMeshSelection = NewSkeletalMesh;
}

void UMultiplayerGameInstance::SaveIDsForPlayers(TArray<FName> NewPlayerSaveIDs)
{
    PlayerSaveIDs = NewPlayerSaveIDs;
}

void UMultiplayerGameInstance::LoadPlayerData(APlayerController *ThisPlayer, FName DataID)
{
    static const FString ContextString(TEXT("Event Context String"));
    ABasicCharacter* Character = Cast<ABasicCharacter>(ThisPlayer->GetPawn());
    if(Character)
    {
        auto Row = Character->CharactersDataTable->FindRow<FCharacterPresets>(DataID, ContextString, true);
        SavedSkeletalMeshSelection = Row->CharacterModel;
        Cast<AMultiplayerPlayerController>(ThisPlayer)->S_SetSavedSkeletalMesh(SavedSkeletalMeshSelection);
    }
}
