// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/MainLevelGameMode.h"
#include "Characters/BasicCharacter.h"
#include "Kismet/GameplayStatics.h"

#include "GameFramework/PlayerStart.h"

void AMainLevelGameMode::PlayerEliminated(ABasicCharacter *EliminatedCharacter, APlayerController *EliminatedPlayerController, APlayerController *AttackerController)
{
    if(EliminatedCharacter)
    {
        EliminatedCharacter->Multicast_ApplyDeath();
    }
}

void AMainLevelGameMode::RequestRespawn(ACharacter *RespawningCharacter, AController *RespawningController)
{
    if(RespawningCharacter)
    {
        RespawningCharacter->Reset();
        RespawningCharacter->Destroy();
    }
    if(RespawningController)
    {
        TArray<AActor*> PlayerSpawns;
        UGameplayStatics::GetAllActorsOfClass(this, APlayerStart::StaticClass(), PlayerSpawns);
        int32 RandomSpawnIndex = FMath::RandRange(0, PlayerSpawns.Num() - 1);
        RestartPlayerAtPlayerStart(RespawningController, PlayerSpawns[RandomSpawnIndex]);
    }
}
