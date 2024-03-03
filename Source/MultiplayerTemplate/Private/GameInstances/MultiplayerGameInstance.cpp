// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInstances/MultiplayerGameInstance.h"
#include "Characters/BasicCharacter.h"

void UMultiplayerGameInstance::LoadSelectedCharacter(APlayerController *ThisPlayer)
{
    GEngine->AddOnScreenDebugMessage(-1, 60.f, FColor::Yellow, FString::Printf(TEXT("Load from game instance")) );
    ABasicCharacter* LoadedCharacterPawn = Cast<ABasicCharacter>(ThisPlayer->GetPawn());
    LoadedCharacterPawn->UpdatedMesh = SavedSkeletalMeshSelection;
    LoadedCharacterPawn->LoadCharacterSelection();
}

void UMultiplayerGameInstance::RegisterCharacterSelection(USkeletalMesh *NewSkeletalMesh)
{
    SavedSkeletalMeshSelection = NewSkeletalMesh;
}