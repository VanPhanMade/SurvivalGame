// Fill out your copyright notice in the Description page of Project Settings.
#include "UI/CharacterSelectionOption.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Button.h"


#include "SaveGame/MultiplayerSaveGameData.h"
#include "Characters/BasicCharacter.h"
#include "GameInstances/MultiplayerGameInstance.h"

#include "Kismet/GameplayStatics.h"

void UCharacterSelectionOption::MenuInit(FName PresetName, USkeletalMesh *_CharacterModel, UTexture2D *_CharacterImage, FText _CharacterDescription, USoundBase *_SelectionAudio)
{
    if(CharacterName)
    {
        CharacterName->SetText(FText::FromName(PresetName));
    }
    if(_CharacterModel)
    {
        CharacterModel = _CharacterModel;
    }
    if(CharacterImage)
    {
        CharacterImage->SetBrushFromTexture(_CharacterImage);
    }
    if(CharacterDescription)
    {
        CharacterDescription->SetText(_CharacterDescription);
    }
    if(_SelectionAudio)
    {
        OnClickAudio = _SelectionAudio;
    }
    if(SelectButton)
    {
        SelectButton->OnClicked.AddDynamic(this, &ThisClass::SelectButtonClicked);
    }
}

bool UCharacterSelectionOption::Initialize() 
{
    if(!Super::Initialize()) return false;
    
    return true;
}

void UCharacterSelectionOption::SelectButtonClicked()
{
    UMultiplayerGameInstance* GameInstance = Cast<UMultiplayerGameInstance>(UGameplayStatics::GetGameInstance(this));
    GameInstance->RegisterCharacterSelection(CharacterModel);
    if(OnClickAudio)
    {
        UGameplayStatics::PlaySound2D(this, OnClickAudio);
    }
    
    // We save only the ID since the datatable is found on every character
    UMultiplayerSaveGameData* SaveData = Cast<UMultiplayerSaveGameData>(UGameplayStatics::CreateSaveGameObject(UMultiplayerSaveGameData::StaticClass()));
    SaveData->SavedSelectionID = FName(*CharacterName->GetText().ToString());
    UGameplayStatics::SaveGameToSlot(SaveData, TEXT("CharacterSelection"), 0);

    Cast<ABasicCharacter>(GetOwningPlayerPawn())->LoadCharacterSelection(CharacterModel);
}
