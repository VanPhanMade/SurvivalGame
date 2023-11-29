// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MultiplayerInGameMenu.h"
#include "GameFramework/PlayerController.h"
#include "Components/Button.h"
#include "GameInstanceSubsystems/MultiplayerSessionsSubsystem.h"
#include "GameFramework/GameModeBase.h"

void UMultiplayerInGameMenu::MenuInit()
{
    AddToViewport();
    SetVisibility(ESlateVisibility::Visible);
    bIsFocusable = true;

    UWorld* World = GetWorld();
    if(World)
    {
        PlayerController = PlayerController == nullptr ? GetOwningPlayer() : PlayerController;
        if(PlayerController)
        {
            FInputModeGameAndUI InputModeData;
            PlayerController->SetInputMode(InputModeData);
            InputModeData.SetWidgetToFocus(TakeWidget());
            InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
            PlayerController->SetShowMouseCursor(true);
        }
    }

    if(ExitGameButton && !ExitGameButton->OnClicked.IsBound())
    {
        ExitGameButton->OnClicked.AddDynamic(this, &ThisClass::ExitGameButtonClicked);
    }

    UGameInstance* GameInstance = GetGameInstance();
    if(GameInstance)
    {
        MultiplayerSessionsSubsystem = GameInstance->GetSubsystem<UMultiplayerSessionsSubsystem>();
    }
}

void UMultiplayerInGameMenu::MenuTearDown()
{
    RemoveFromParent();

    UWorld* World = GetWorld();
    if(World)
    {
        PlayerController = PlayerController == nullptr ? World->GetFirstPlayerController() : PlayerController;
        if(PlayerController)
        {
            FInputModeGameOnly InputModeData;
            PlayerController->SetInputMode(InputModeData);
            PlayerController->SetShowMouseCursor(false);
        }
    }

    if(ExitGameButton && ExitGameButton->OnClicked.IsBound())
    {
        ExitGameButton->OnClicked.RemoveDynamic(this, &ThisClass::ExitGameButtonClicked);
    }
}

bool UMultiplayerInGameMenu::Initialize()
{
    if(!Super::Initialize()) return false;

    return true;
}

void UMultiplayerInGameMenu::ExitGameButtonClicked()
{
    ExitGameButton->SetIsEnabled(false);

    if(MultiplayerSessionsSubsystem)
    {
        MultiplayerSessionsSubsystem->DestroySession();
        UWorld* World = GetWorld();
        if(World)
        {   
            //will return nullptr if we're a client
            AGameModeBase* GameMode = World->GetAuthGameMode<AGameModeBase>();
            if(GameMode)
            {
                GameMode->ReturnToMainMenuHost();
            }
            else
            {
                PlayerController = PlayerController == nullptr ? GetOwningPlayer() : PlayerController;
                if(PlayerController)
                {
                    PlayerController->ClientReturnToMainMenuWithTextReason(FText());
                }
            }
        }
    }
}
