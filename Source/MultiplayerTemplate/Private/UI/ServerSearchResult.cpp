// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ServerSearchResult.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "OnlineSessionSettings.h"
#include "GameInstanceSubsystems/MultiplayerSessionsSubsystem.h"
#include "OnlineSubsystemUtils.h"

void UServerSearchResult::MenuInit(FOnlineSessionSearchResult& SessionResultRef)
{
    SessionResult = &SessionResultRef;
    FOnlineSession Session = SessionResult->Session;
    SessionNameText->SetText(FText::FromString(Session.OwningUserName.LeftChop(15)));

    int32 NumberOfMaxConnections = Session.SessionSettings.NumPublicConnections;
    int32 NumberOfOpenConnections = Session.NumOpenPublicConnections;
    int32 NumberOfCurrentPlayers = NumberOfMaxConnections - NumberOfOpenConnections;
    const FString PlayersInSession = FString::Printf(TEXT("%d / %d Players"), NumberOfCurrentPlayers, NumberOfMaxConnections);
    PlayerAmountText->SetText(FText::FromString(PlayersInSession));

    int32 Ping = SessionResult->PingInMs;
    const FString PingString = FString::Printf(TEXT("%d m/s"), Ping);
    PingText->SetText(FText::FromString(PingString));
}


bool UServerSearchResult::Initialize()
{
    if(!Super::Initialize()) return false;
    UGameInstance* GameInstance = GetGameInstance();
    if(GameInstance)
    {
        MultiplayerSessionsSubsystem = GameInstance->GetSubsystem<UMultiplayerSessionsSubsystem>();
    }

    if(JoinServerButton)
    {
        JoinServerButton->OnClicked.AddDynamic(this, &ThisClass::JoinServerButtonClicked);
    }
    
    return true;
}

void UServerSearchResult::JoinServerButtonClicked()
{
    JoinServerButton->SetIsEnabled(false);
    if(SessionResult == nullptr || !SessionResult->IsValid())
    {
        MultiplayerSessionsSubsystem->MultiplayerOnFindSessionsComplete.AddUObject(this, &ThisClass::OnFindSessions);
        MultiplayerSessionsSubsystem->FindSessions(50);
    }
    else
    {
        MultiplayerSessionsSubsystem->JoinSession(*SessionResult);
    }
    
}

void UServerSearchResult::OnJoinSession(EOnJoinSessionCompleteResult::Type Result)
{
    JoinServerButton->SetIsEnabled(true);
    if(Result == EOnJoinSessionCompleteResult::Success)
    {
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
    }
}

void UServerSearchResult::OnFindSessions(const TArray<FOnlineSessionSearchResult> &SessionResults, bool bWasSuccessful)
{
    if(bWasSuccessful == false || MultiplayerSessionsSubsystem == nullptr || SessionResults.Num() == 0) 
    {
        GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString::Printf(TEXT("Sessions corrupted")));
        return;
    }

    for(auto _SessionResult : SessionResults)
    {
        FText SessionOwner = FText::FromString(_SessionResult.Session.OwningUserName.LeftChop(15));
        if(SessionOwner.ToString().ToLower() == SessionNameText->GetText().ToString().ToLower())
        {
            if(MultiplayerSessionsSubsystem)
            {
                MultiplayerSessionsSubsystem->MultiplayerOnJoinSessionComplete.AddUObject(this, &ThisClass::OnJoinSession);
            }
            MultiplayerSessionsSubsystem->JoinSession(_SessionResult);
            return;
        }
    }
}
