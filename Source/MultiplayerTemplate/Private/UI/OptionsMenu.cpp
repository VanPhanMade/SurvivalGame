// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/OptionsMenu.h"
#include "Components/Button.h"
#include "UI/MultiplayerStartMenu.h"

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

    return true;
}

void UOptionsMenu::ReturnButtonClicked()
{
    if(!IsValid(StartMenuWidget)) return;

    GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Blue, FString::Printf(TEXT("Return Button Clicked")));
    
    UMultiplayerStartMenu* StartMenu = CreateWidget<UMultiplayerStartMenu>(GetWorld(), StartMenuWidget);
    StartMenu->MenuInit();
    RemoveFromParent();
}

void UOptionsMenu::ApplyButtonClicked()
{
    GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Blue, FString::Printf(TEXT("Apply Button Clicked")));
}
