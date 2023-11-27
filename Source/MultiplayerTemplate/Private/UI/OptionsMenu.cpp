// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/OptionsMenu.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "UI/MultiplayerStartMenu.h"
#include "GameFramework/GameUserSettings.h"
#include "Math/UnrealMathUtility.h"

void UOptionsMenu::MenuInit()
{
    AddToViewport();
    SetVisibility(ESlateVisibility::Visible);
    bIsFocusable = true;
    
    // When the UI is initialized on the screen, show the cursor 
    UWorld* World = GetWorld();
    if(World)
    {
        APlayerController* PlayerController = World->GetFirstPlayerController();
        if(PlayerController)
        {
            FInputModeUIOnly InputModeData;
            InputModeData.SetWidgetToFocus(TakeWidget());
            InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
            PlayerController->SetInputMode(InputModeData);
            PlayerController->SetShowMouseCursor(true);
        }
    }

    if(ReturnButton && !ReturnButton->OnClicked.IsBound())
    {
        ReturnButton->OnClicked.AddDynamic(this, &ThisClass::ReturnButtonClicked);
    }

    if(ApplyButton && !ApplyButton->OnClicked.IsBound())
    {
        ApplyButton->OnClicked.AddDynamic(this, &ThisClass::ApplyButtonClicked);
    }

    if(ScalabilityLevelUpButton && !ScalabilityLevelUpButton->OnClicked.IsBound())
    {
        ScalabilityLevelUpButton->OnClicked.AddDynamic(this, &ThisClass::IncreaseScalability);
    }

    if(ScalabilityLevelDownButton && !ScalabilityLevelDownButton->OnClicked.IsBound())
    {
        ScalabilityLevelDownButton->OnClicked.AddDynamic(this, &ThisClass::DecreaseScalability);
    }

    UGameUserSettings* UserSettings = UGameUserSettings::GetGameUserSettings();
    CurrentScalabilityLevel = UserSettings->GetOverallScalabilityLevel();
    SetScalabilityLevelText();
    
}
bool UOptionsMenu::Initialize()
{
    if(!Super::Initialize()) return false;

    if(ReturnButton && !ReturnButton->OnClicked.IsBound())
    {
        ReturnButton->OnClicked.AddDynamic(this, &ThisClass::ReturnButtonClicked);
    }

    if(ApplyButton && !ApplyButton->OnClicked.IsBound())
    {
        ApplyButton->OnClicked.AddDynamic(this, &ThisClass::ApplyButtonClicked);
    }

    if(ScalabilityLevelUpButton && !ScalabilityLevelUpButton->OnClicked.IsBound())
    {
        ScalabilityLevelUpButton->OnClicked.AddDynamic(this, &ThisClass::IncreaseScalability);
    }

    if(ScalabilityLevelDownButton && !ScalabilityLevelDownButton->OnClicked.IsBound())
    {
        ScalabilityLevelDownButton->OnClicked.AddDynamic(this, &ThisClass::DecreaseScalability);
    }

    return true;
}

void UOptionsMenu::ReturnButtonClicked()
{
    if(!IsValid(StartMenuWidget)) return;
    
    UMultiplayerStartMenu* StartMenu = CreateWidget<UMultiplayerStartMenu>(GetWorld(), StartMenuWidget);
    StartMenu->MenuInit();
    RemoveFromParent();
}

void UOptionsMenu::ApplyButtonClicked()
{
    UGameUserSettings* UserSettings = UGameUserSettings::GetGameUserSettings();
    UserSettings->SetOverallScalabilityLevel(CurrentScalabilityLevel);
}

void UOptionsMenu::IncreaseScalability()
{
    CurrentScalabilityLevel++;
    CurrentScalabilityLevel = FMath::Clamp(CurrentScalabilityLevel, 0, 4);
    SetScalabilityLevelText();
}

void UOptionsMenu::DecreaseScalability()
{
    CurrentScalabilityLevel--;
    CurrentScalabilityLevel = FMath::Clamp(CurrentScalabilityLevel, 0, 4);
    SetScalabilityLevelText();
}

void UOptionsMenu::SetScalabilityLevelText()
{
    switch (CurrentScalabilityLevel)
    {
    case 0:
        ScalabilityLevelText->SetText(FText::FromString("Low"));
        break;
    case 1:
        ScalabilityLevelText->SetText(FText::FromString("Medium"));
        break;
    case 2:
        ScalabilityLevelText->SetText(FText::FromString("High"));
        break;
    case 3:
        ScalabilityLevelText->SetText(FText::FromString("Epic"));
        break;
    case 4:
        ScalabilityLevelText->SetText(FText::FromString("Cinematic"));
        break;
    case -1:
        ScalabilityLevelText->SetText(FText::FromString("Custom"));
        break;
    default:
        ScalabilityLevelText->SetText(FText::FromString("Custom"));
        break;
    }
}
