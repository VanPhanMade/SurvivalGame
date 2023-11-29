// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInstanceSubsystems/MultiplayerSessionsSubsystem.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "OnlineSubsystemUtils.h"

UMultiplayerSessionsSubsystem::UMultiplayerSessionsSubsystem() :
    CreateSessionCompleteDelegate(FOnCreateSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnCreateSessionComplete)),
    FindSessionsCompleteDelegate(FOnFindSessionsCompleteDelegate::CreateUObject(this, &ThisClass::OnFindSessionsComplete)),
    JoinSessionCompleteDelegate(FOnJoinSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnJoinSessionComplete)),
    DestroySessionCompleteDelegate(FOnDestroySessionCompleteDelegate::CreateUObject(this, &ThisClass::OnDestroySessionComplete)),
    StartSessionCompleteDelegate(FOnStartSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnStartSessionComplete)),
    UpdateSessionCompleteDelegate(FOnUpdateSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnUpdateSessionCompleted))
{
    IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
    if(Subsystem)
    {
        SessionInterface = Online::GetSessionInterface(GetWorld());
        if(GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,
				15.f,
				FColor::Blue,
				FString::Printf(TEXT("Found subsystem %s "), *Subsystem->GetSubsystemName().ToString())
			);
		}
    }
    else
    {
        if(GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,
				15.f,
				FColor::Red,
				FString::Printf(TEXT("No multiplayer subsystem detected!"))
			);
		}
    }
}

void UMultiplayerSessionsSubsystem::CreateSession(int32 NumPublicConnections, FString MatchType)
{
    if(!SessionInterface.IsValid()) return MultiplayerOnCreateSessionComplete.Broadcast(false);

    auto ExistingSession = SessionInterface->GetNamedSession(NAME_GameSession);
    if(ExistingSession != nullptr)
    {
        bCreateSessionDestroy = true;
        LastNumPublicConnections = NumPublicConnections;
        LastMatchType = MatchType;
        DestroySession();
    }

    LastSessionSettings = MakeShareable(new FOnlineSessionSettings());
    LastSessionSettings->bIsLANMatch = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL" ? true : false;
    LastSessionSettings->NumPrivateConnections = 0;
    LastSessionSettings->NumPublicConnections = NumPublicConnections;
    LastSessionSettings->bAllowInvites = true;
    LastSessionSettings->bAllowJoinInProgress = true;
    LastSessionSettings->bAllowJoinViaPresence = true;
    //LastSessionSettings->bAllowJoinViaPresenceFriendsOnly = false;
    LastSessionSettings->bShouldAdvertise = true;
    LastSessionSettings->bIsDedicated = false;
    LastSessionSettings->bUsesPresence = true;
    LastSessionSettings->bUseLobbiesIfAvailable = true;
    LastSessionSettings->BuildUniqueId = 1;
    LastSessionSettings->Set(FName("MatchType"), MatchType, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

    CreateSessionCompleteDelegateHandle = SessionInterface->AddOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegate);

    const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
    SessionInterface->CreateSession(*LocalPlayer->GetPreferredUniqueNetId(), NAME_GameSession, *LastSessionSettings);
}

void UMultiplayerSessionsSubsystem::FindSessions(int32 MaxSearchResults)
{
    if(!SessionInterface.IsValid()) return MultiplayerOnFindSessionsComplete.Broadcast(TArray<FOnlineSessionSearchResult>(), false);
    
    FindSessionsCompleteDelegateHandle = SessionInterface->AddOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegate);
    
    LastSessionSearch = MakeShareable(new FOnlineSessionSearch());
    LastSessionSearch->MaxSearchResults = MaxSearchResults;
    LastSessionSearch->bIsLanQuery = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL" ? true : false;
    LastSessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

    const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	SessionInterface->FindSessions(*LocalPlayer->GetPreferredUniqueNetId(), LastSessionSearch.ToSharedRef());
}

void UMultiplayerSessionsSubsystem::JoinSession(const FOnlineSessionSearchResult &SessionResult)
{
    if(!SessionInterface.IsValid()) return MultiplayerOnJoinSessionComplete.Broadcast(EOnJoinSessionCompleteResult::UnknownError);

    JoinSessionCompleteDelegateHandle = SessionInterface->AddOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegate);
    
    const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
    SessionInterface->JoinSession(*LocalPlayer->GetPreferredUniqueNetId(), NAME_GameSession, SessionResult);
}

void UMultiplayerSessionsSubsystem::DestroySession()
{
    if(!SessionInterface.IsValid()) return MultiplayerOnDestroySessionComplete.Broadcast(false);

    DestroySessionCompleteDelegateHandle = SessionInterface->AddOnEndSessionCompleteDelegate_Handle(DestroySessionCompleteDelegate);
    SessionInterface->DestroySession(NAME_GameSession);
}

void UMultiplayerSessionsSubsystem::StartSession()
{
    if(!SessionInterface.IsValid()) return MultiplayerOnStartSessionComplete.Broadcast(false);

    StartSessionCompleteDelegateHandle = SessionInterface->AddOnStartSessionCompleteDelegate_Handle(StartSessionCompleteDelegate);

    SessionInterface->StartSession(NAME_GameSession);
}

void UMultiplayerSessionsSubsystem::UpdateSession()
{
	if (!SessionInterface.IsValid()) return MultiplayerOnUpdateSessionComplete.Broadcast(false);

    UpdatedSessionSettings = MakeShareable(new FOnlineSessionSettings(*LastSessionSettings));
    UpdatedSessionSettings->Set(FName("MatchType"), LastMatchType, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

    UpdateSessionCompleteDelegateHandle = SessionInterface->AddOnUpdateSessionCompleteDelegate_Handle(UpdateSessionCompleteDelegate);

    SessionInterface->UpdateSession(NAME_GameSession, *UpdatedSessionSettings);
}

void UMultiplayerSessionsSubsystem::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
    if(!SessionInterface) return;
    SessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegateHandle);
    MultiplayerOnCreateSessionComplete.Broadcast(bWasSuccessful);

    if(bWasSuccessful)
    {
        UWorld* World = GetWorld();
        if(World)
        {
            if(GEngine)
            {
                GEngine->AddOnScreenDebugMessage(
                    -1,
                    15.f,
                    FColor::Yellow,
                    FString::Printf(TEXT("Traveling to lobby."))
            );
            World->ServerTravel(FString("/Game/Maps/Lobby?listen"));
        }


    }
    }
    else
    {
        if(GEngine)
        {
            GEngine->AddOnScreenDebugMessage(
                -1,
                15.f,
                FColor::Red,
                FString::Printf(TEXT("Failed to create a session."))
            );
        }
    }
}

void UMultiplayerSessionsSubsystem::OnFindSessionsComplete(bool bWasSuccessful)
{
    if(!SessionInterface.IsValid()) return;
    SessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegateHandle);
    
    if(bWasSuccessful == false || LastSessionSearch->SearchResults.Num() == 0) 
    {
        MultiplayerOnFindSessionsComplete.Broadcast(LastSessionSearch->SearchResults, false);
        return;
    }
    MultiplayerOnFindSessionsComplete.Broadcast(LastSessionSearch->SearchResults, bWasSuccessful);


    for (auto Result : LastSessionSearch->SearchResults)
    {
        FString SettingsMatchType;
        Result.Session.SessionSettings.Get(FName("MatchType"), SettingsMatchType);
        if(SettingsMatchType == LastMatchType)
        {
            LastSearchResult = Result;
            if(GEngine)
		    {
		    	GEngine->AddOnScreenDebugMessage(
		    		-1,
		    		15.f,
		    		FColor::Cyan,
		    		FString::Printf(TEXT("Host: %s"), *Result.Session.OwningUserName)
		    	);
		    }

            return JoinSession(Result);
        }
    }
}

void UMultiplayerSessionsSubsystem::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
    if(!SessionInterface.IsValid()) return;
    SessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegateHandle);
    MultiplayerOnJoinSessionComplete.Broadcast(Result);

    
}

void UMultiplayerSessionsSubsystem::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
    if(!SessionInterface.IsValid()) return;
    SessionInterface->ClearOnDestroySessionCompleteDelegate_Handle(DestroySessionCompleteDelegateHandle);
    MultiplayerOnDestroySessionComplete.Broadcast(bWasSuccessful);

    if(bWasSuccessful && bCreateSessionDestroy)
    {
        bCreateSessionDestroy = false;
        CreateSession(LastNumPublicConnections, LastMatchType);
    }
}

void UMultiplayerSessionsSubsystem::OnStartSessionComplete(FName SessionName, bool bWasSuccessful)
{
    MultiplayerOnStartSessionComplete.Broadcast(bWasSuccessful);
    SessionInterface->ClearOnStartSessionCompleteDelegate_Handle(StartSessionCompleteDelegateHandle);

    UWorld* World = GetWorld();
    if(World) World->ServerTravel(FString("/Game/Maps/Main?listen"), true);

}

void UMultiplayerSessionsSubsystem::OnUpdateSessionCompleted(FName SessionName, bool bWasSuccessful)

{
    if(!SessionInterface.IsValid()) return MultiplayerOnUpdateSessionComplete.Broadcast(false);
    SessionInterface->ClearOnUpdateSessionCompleteDelegate_Handle(UpdateSessionCompleteDelegateHandle);

    if(bWasSuccessful)
    {
        LastSessionSettings = UpdatedSessionSettings;
        MultiplayerOnUpdateSessionComplete.Broadcast(bWasSuccessful);
    }
    else
    {
        MultiplayerOnUpdateSessionComplete.Broadcast(false);
    }
}

bool UMultiplayerSessionsSubsystem::TryTravelToCurrentSession()
{
    SessionInterface = SessionInterface.IsValid() ? SessionInterface : Online::GetSessionInterface(GetWorld());
	if (!SessionInterface.IsValid()) return false;

	FString connectString;
	if (!SessionInterface->GetResolvedConnectString(NAME_GameSession, connectString)) return false;

	APlayerController* playerController = GetWorld()->GetFirstPlayerController();
	playerController->ClientTravel(connectString, TRAVEL_Absolute);
	return true;
}
