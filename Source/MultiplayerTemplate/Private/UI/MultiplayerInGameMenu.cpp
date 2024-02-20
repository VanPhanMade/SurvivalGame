// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MultiplayerInGameMenu.h"
#include "GameFramework/PlayerController.h"
#include "Components/Button.h"
#include "GameInstanceSubsystems/MultiplayerSessionsSubsystem.h"
#include "GameFramework/GameModeBase.h"
#include "Characters/BasicCharacter.h"
#include "Kismet/KismetSystemLibrary.h"

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

    if(ContinueGameButton && !ContinueGameButton->OnClicked.IsBound())
    {
        ContinueGameButton->OnClicked.AddDynamic(this, &ThisClass::ContinueGameButtonClicked);
    }

    if(ExitServerButton && !ExitServerButton->OnClicked.IsBound())
    {
        ExitServerButton->OnClicked.AddDynamic(this, &ThisClass::ExitServerButtonClicked);
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

    if(ContinueGameButton && !ContinueGameButton->OnClicked.IsBound())
    {
        ContinueGameButton->OnClicked.RemoveDynamic(this, &ThisClass::ContinueGameButtonClicked);
    }

    if(ExitServerButton && ExitServerButton->OnClicked.IsBound())
    {
        ExitServerButton->OnClicked.RemoveDynamic(this, &ThisClass::ExitServerButtonClicked);
    }

    if(ExitGameButton && !ExitGameButton->OnClicked.IsBound())
    {
        ExitGameButton->OnClicked.RemoveDynamic(this, &ThisClass::ExitGameButtonClicked);
    }
}

bool UMultiplayerInGameMenu::Initialize()
{
    if(!Super::Initialize()) return false;

    return true;
}

void UMultiplayerInGameMenu::ContinueGameButtonClicked()
{
    ABasicCharacter* OwningPlayer = Cast<ABasicCharacter>(GetOwningPlayer()->GetPawn());
    OwningPlayer->OpenInGameMenu();
}

void UMultiplayerInGameMenu::ExitServerButtonClicked()
{
    ExitServerButton->SetIsEnabled(false);

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

void UMultiplayerInGameMenu::ExitGameButtonClicked()
{
    UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, true);
}
