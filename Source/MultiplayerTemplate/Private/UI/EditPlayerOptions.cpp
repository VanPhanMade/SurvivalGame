// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/EditPlayerOptions.h"
#include "Components/ScrollBox.h"
#include "Engine/DataTable.h"
#include "UI/CharacterSelectionOption.h"
#include "DataTables/CharacterPresetsDataTable.h"
#include "Components/Button.h"

void UEditPlayerOptions::MenuInit()
{
    if(BackButton)
    {
        BackButton->OnClicked.AddDynamic(this, &ThisClass::BackButtonClicked);
    }
}

void UEditPlayerOptions::RefreshOptions()
{
    static const FString ContextString(TEXT("Event Context String"));
    if(CharacterOptionsScrollbox != nullptr)
    {
        CharacterOptionsScrollbox->ClearChildren();
        if(CharactersDataTable)
        {
            for(auto Rows : CharactersDataTable->GetRowNames())
            {
                UCharacterSelectionOption* CurrentSelection = CreateWidget<UCharacterSelectionOption>(GetWorld(), PlayerOptionWidget);
                auto Row = CharactersDataTable->FindRow<FCharacterPresets>(Rows, ContextString, true);
                CurrentSelection->MenuInit(Row->PresetName, Row->CharacterModel, Row->CharacterImage, Row->CharacterDescription, Row->SelectionAudio);
                CharacterOptionsScrollbox->AddChild(CurrentSelection);
            }
        }
    }
}

bool UEditPlayerOptions::Initialize()
{
    if(!Super::Initialize()) return false;
    
    return true;
}

void UEditPlayerOptions::BackButtonClicked()
{
    OnBackButtonEditPlayerClicked.Broadcast();
}
