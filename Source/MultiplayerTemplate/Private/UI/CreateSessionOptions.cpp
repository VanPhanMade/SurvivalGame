// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CreateSessionOptions.h"
#include "Components/Button.h"
#include "Components/CheckBox.h"
#include "Components/EditableTextBox.h"
#include "Kismet/KismetStringLibrary.h"
#include "GameInstanceSubsystems/MultiplayerSessionsSubsystem.h"

void UCreateSessionOptions::MenuInit()
{
    UGameInstance* GameInstance = GetGameInstance();
    if(GameInstance)
    {
        MultiplayerSessionsSubsystem = GameInstance->GetSubsystem<UMultiplayerSessionsSubsystem>();
    }

    if(MultiplayerSessionsSubsystem)
    {
        MultiplayerSessionsSubsystem->MultiplayerOnCreateSessionComplete.AddDynamic(this, &ThisClass::OnCreateSession);
    }

    if(MaxPlayerAmountTextBox != nullptr) MaxPlayerAmountTextBox->SetText(FText::FromString(FString::Printf(TEXT("10"))));

}

void UCreateSessionOptions::MenuTearDown()
{
    
}

bool UCreateSessionOptions::Initialize()
{
    if(!Super::Initialize()) return false;

    if(BackButton)
    {
        BackButton->OnClicked.AddDynamic(this, &ThisClass::BackButtonClicked);
    }
    if(CreateSessionButton)
    {
        CreateSessionButton->OnClicked.AddDynamic(this, &ThisClass::CreateSessionButtonClicked);
    }

    return true;
}

void UCreateSessionOptions::OnCreateSession(bool bWasSuccessful)
{
    BackButton->SetIsEnabled(true);
    CreateSessionButton->SetIsEnabled(true);
    EnableLanCheckbox->SetIsEnabled(true);
    MaxPlayerAmountTextBox->SetIsEnabled(true);
}

void UCreateSessionOptions::BackButtonClicked()
{
    OnBackButtonCreateSessionClicked.Broadcast();
}

void UCreateSessionOptions::CreateSessionButtonClicked()
{

    if(!MaxPlayerAmountTextBox->Text.IsNumeric() || UKismetStringLibrary::Conv_StringToInt(MaxPlayerAmountTextBox->GetText().ToString()) > 20)
    {
        MaxPlayerAmountTextBox->SetText(FText::FromString(FString::Printf(TEXT("Invalid number"))));
    }
    else
    {
        BackButton->SetIsEnabled(false);
        CreateSessionButton->SetIsEnabled(false);
        EnableLanCheckbox->SetIsEnabled(false);
        MaxPlayerAmountTextBox->SetIsEnabled(false);
        if(MultiplayerSessionsSubsystem)
        {
            MultiplayerSessionsSubsystem->CreateSession(UKismetStringLibrary::Conv_StringToInt(MaxPlayerAmountTextBox->GetText().ToString()), TEXT("FreeForAll"), EnableLanCheckbox->IsChecked());
        }
    }
    

}
