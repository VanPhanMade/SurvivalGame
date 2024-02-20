// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/LobbyGameMode.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"
#include "Characters/BasicCharacter.h"


void ALobbyGameMode::PostLogin(APlayerController *NewPlayer)
{
    Super::PostLogin(NewPlayer);

    if(GameState)
    {
        int32 NumberOfPlayers = GameState.Get()->PlayerArray.Num();
        if(GEngine)
        {
            GEngine->AddOnScreenDebugMessage(
				1,
				60.f,
				FColor::Green,
				FString::Printf(TEXT("Players in lobby %d"), NumberOfPlayers)
			);

            APlayerState* PlayerState = NewPlayer->GetPlayerState<APlayerState>();
            if(PlayerState)
            {
                FString PlayerName = PlayerState->GetPlayerName();
                GEngine->AddOnScreenDebugMessage(
				    -1,
				    60.f,
				    FColor::Yellow,
				    FString::Printf(TEXT("%s has joined the game!"), *PlayerName)
			    );

                //Cast<ABasicCharacter>(PlayerState->GetOwningController()->GetPawn())->LoadCharacterSelection();
            }
            
        }
    }
}

void ALobbyGameMode::Logout(AController *Exiting)
{
    Super::Logout(Exiting);

    if(GameState)
    {
        int32 NumberOfPlayers = GameState.Get()->PlayerArray.Num();
        if(GEngine)
        {
            GEngine->AddOnScreenDebugMessage(
				1,
				60.f,
				FColor::Green,
				FString::Printf(TEXT("Players in lobby %d"), NumberOfPlayers - 1)
			);

            APlayerState* PlayerState = Exiting->GetPlayerState<APlayerState>();
            if(PlayerState)
            {
                FString PlayerName = PlayerState->GetPlayerName();
                GEngine->AddOnScreenDebugMessage(
				    -1,
				    60.f,
				    FColor::Yellow,
				    FString::Printf(TEXT("%s has exitted the game!"), *PlayerName)
			    );
            }
            
        }
    }
}
