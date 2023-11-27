// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MultiplayerStartMenu.h"
#include "Components/Button.h"
#include "OnlineSessionSettings.h"
#include "GameInstanceSubsystems/MultiplayerSessionsSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSubsystemUtils.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UI/OptionsMenu.h"

void UMultiplayerStartMenu::MenuInit(int32 NumberOfPublicConnections, FString TypeOfMatch)
{
    NumberPublicConnections = NumberOfPublicConnections;
    MatchType = TypeOfMatch;

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

    UGameInstance* GameInstance = GetGameInstance();
    if(GameInstance)
    {
        MultiplayerSessionsSubsystem = GameInstance->GetSubsystem<UMultiplayerSessionsSubsystem>();
    }

    if(MultiplayerSessionsSubsystem)
    {
        MultiplayerSessionsSubsystem->MultiplayerOnCreateSessionComplete.AddDynamic(this, &ThisClass::OnCreateSession);
        MultiplayerSessionsSubsystem->MultiplayerOnFindSessionsComplete.AddUObject(this, &ThisClass::OnFindSessions);
        MultiplayerSessionsSubsystem->MultiplayerOnJoinSessionComplete.AddUObject(this, &ThisClass::OnJoinSession);
        MultiplayerSessionsSubsystem->MultiplayerOnDestroySessionComplete.AddDynamic(this, &ThisClass::OnDestroySession);
        MultiplayerSessionsSubsystem->MultiplayerOnStartSessionComplete.AddDynamic(this, &ThisClass::OnStartSession);
    }
}

bool UMultiplayerStartMenu::Initialize()
{
    if(!Super::Initialize()) return false;

    if(HostButton)
    {
        HostButton->OnClicked.AddDynamic(this, &ThisClass::HostButtonClicked);
    }

    if(JoinButton)
    {
        JoinButton->OnClicked.AddDynamic(this, &ThisClass::JoinButtonClicked);
    }

    if(OptionsButton)
    {
        OptionsButton->OnClicked.AddDynamic(this, &ThisClass::OptionsButtonClicked);
    }

    if(QuitButton)
    {
        QuitButton->OnClicked.AddDynamic(this, &ThisClass::QuitButtonClicked);
    }

    return true;
}

void UMultiplayerStartMenu::NativeDestruct()
{
    MenuTearDown();
 
	Super::NativeDestruct();
}

void UMultiplayerStartMenu::OnCreateSession(bool bWasSuccessful)
{
    if(bWasSuccessful) GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, FString::Printf(TEXT("Session created menu callback!")));

    HostButton->SetIsEnabled(true);
    JoinButton->SetIsEnabled(true);
}

void UMultiplayerStartMenu::OnFindSessions(const TArray<FOnlineSessionSearchResult> &SessionResults, bool bWasSuccessful)
{
    if(bWasSuccessful == false || MultiplayerSessionsSubsystem == nullptr) 
    {
        HostButton->SetIsEnabled(true);
        JoinButton->SetIsEnabled(true);
        GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString::Printf(TEXT("Sessions searched failed menu callback!")));
        return;
    }
    
    if(bWasSuccessful)
    {
        GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, FString::Printf(TEXT("Sessions searched menu callback!")));
        return;
    }
}

void UMultiplayerStartMenu::OnJoinSession(EOnJoinSessionCompleteResult::Type Result)
{
    if(Result == EOnJoinSessionCompleteResult::Success)
    {
        GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, FString::Printf(TEXT("Traveling.......")));
        if(MultiplayerSessionsSubsystem->TryTravelToCurrentSession())
        {
            GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, FString::Printf(TEXT("Traveled complete!")));
        }
        else
        {
            GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString::Printf(TEXT("Failed to travel.....")));
        }
    }
    else
    {
        GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString::Printf(TEXT("Session joined failed menu callback!")));
        HostButton->SetIsEnabled(true);
        JoinButton->SetIsEnabled(true);
    }
    
}

void UMultiplayerStartMenu::OnDestroySession(bool bWasSuccessful)
{
    if(bWasSuccessful) 
    {
        GEngine->AddOnScreenDebugMessage(
            -1,
            15.f,
            FColor::Green,
            FString::Printf(TEXT("Session destroyed menu callback!"))
        );
    }
    else
    {
        GEngine->AddOnScreenDebugMessage(
            -1,
            15.f,
            FColor::Green,
            FString::Printf(TEXT("Session destroyed failed menu callback!"))
        );
    }
}

void UMultiplayerStartMenu::OnStartSession(bool bWasSuccessful)
{

}

void UMultiplayerStartMenu::HostButtonClicked()
{
    HostButton->SetIsEnabled(false);
    JoinButton->SetIsEnabled(false);
    if(MultiplayerSessionsSubsystem)
    {
        MultiplayerSessionsSubsystem->CreateSession(NumberPublicConnections, MatchType);
    }
}

void UMultiplayerStartMenu::JoinButtonClicked()
{
    HostButton->SetIsEnabled(false);
    JoinButton->SetIsEnabled(false);
    if(MultiplayerSessionsSubsystem)
    {
        MultiplayerSessionsSubsystem->FindSessions(100);
    }
}

void UMultiplayerStartMenu::OptionsButtonClicked()
{
    if(!IsValid(OptionsMenuWidget)) return;

    UOptionsMenu* Options = CreateWidget<UOptionsMenu>(GetWorld(), OptionsMenuWidget);
    Options->MenuInit();

    RemoveFromParent();
}

void UMultiplayerStartMenu::QuitButtonClicked()
{
    UKismetSystemLibrary::QuitGame(GetWorld(), GetOwningPlayer(), EQuitPreference::Quit, true);
}

void UMultiplayerStartMenu::MenuTearDown()
{
    RemoveFromParent();
    UWorld* World = GetWorld();
    if(World)
    {
        APlayerController* PlayerController = World->GetFirstPlayerController();
        if(PlayerController)
        {
            FInputModeGameOnly InputModeData;
            PlayerController->SetInputMode(InputModeData);
            PlayerController->SetShowMouseCursor(false);
        }
    }
}
