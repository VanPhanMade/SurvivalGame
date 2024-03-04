// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/LobbyGameMode.h"

#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"
#include "Characters/BasicCharacter.h"
#include "GameInstances/MultiplayerGameInstance.h"
#include "PlayerControllers/MultiplayerPlayerController.h"
#include "SaveGame/MultiplayerSaveGameData.h"

#include "Kismet/GameplayStatics.h"

void ALobbyGameMode::PostLogin(APlayerController *NewPlayer)
{
    Super::PostLogin(NewPlayer);

    if(GameState)
    {
        UMultiplayerGameInstance* GameInstance = Cast<UMultiplayerGameInstance>(UGameplayStatics::GetGameInstance(NewPlayer));
        if(GameInstance)
        {
            Cast<AMultiplayerPlayerController>(NewPlayer)->S_LoadSavedDataToLobbyGM();
            //Cast<AMultiplayerPlayerController>(NewPlayer)->S_SetSavedSkeletalMesh(GameInstance->SavedSkeletalMeshSelection);
        }
    }
    //int32 NumberOfPlayers = GameState.Get()->PlayerArray.Num();
}

void ALobbyGameMode::Logout(AController *Exiting)
{
    Super::Logout(Exiting);

    if(GameState)
    {
        int32 NumberOfPlayers = GameState.Get()->PlayerArray.Num();
        if(GEngine)
        {
            GEngine->AddOnScreenDebugMessage(1, 60.f, FColor::Green, FString::Printf(TEXT("Players in lobby %d"), NumberOfPlayers - 1));

            APlayerState* PlayerState = Exiting->GetPlayerState<APlayerState>();
            if(PlayerState)
            {
                FString PlayerName = PlayerState->GetPlayerName();
                GEngine->AddOnScreenDebugMessage(-1, 60.f, FColor::Yellow, FString::Printf(TEXT("%s has exitted the game!"), *PlayerName));
            }
        }
    }
}

void ALobbyGameMode::SaveNewPlayerSaveDataToGameInstance(APlayerController *NewLoggedInPlayer, FName SaveDataID)
{
    int32 index = 0;
    if(PlayerSaves.Max() == 0 || PlayerSaves.Num() == 0)
    {
        for (size_t i = 0; i < 30; i++)
        {
            PlayerSaves.Add(FName(TEXT("")));
        }
        
    }
    for(auto Player : GameState.Get()->PlayerArray)
    {
        if(Player->GetOwningController() == NewLoggedInPlayer)
        {
            PlayerSaves[index] = SaveDataID;
        }
        index++;
    }

    UMultiplayerGameInstance* GameInstance = Cast<UMultiplayerGameInstance>(UGameplayStatics::GetGameInstance(NewLoggedInPlayer));
    // On Complete save the new array data to our array on our game instance
    GameInstance->SaveIDsForPlayers(PlayerSaves);
    GameInstance->LoadPlayerData(NewLoggedInPlayer, SaveDataID);
}
