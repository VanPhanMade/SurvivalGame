// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ServerSearchList.h"
#include "Components/ScrollBox.h"
#include "Components/Button.h"
#include "UI/ServerSearchResult.h"
#include "GameInstanceSubsystems/MultiplayerSessionsSubsystem.h"

void UServerSearchList::MenuInit()
{
    UGameInstance* GameInstance = GetGameInstance();
    if(GameInstance)
    {
        MultiplayerSessionsSubsystem = GameInstance->GetSubsystem<UMultiplayerSessionsSubsystem>();
    }

    if(MultiplayerSessionsSubsystem)
    {
        MultiplayerSessionsSubsystem->MultiplayerOnFindSessionsComplete.AddUObject(this, &ThisClass::OnFindSessions);
    }

    
}

void UServerSearchList::MenuTearDown()
{

}

bool UServerSearchList::Initialize()
{
    if(!Super::Initialize()) return false;

    if(BackButton)
    {
        BackButton->OnClicked.AddDynamic(this, &ThisClass::BackButtonClicked);
    }
    if(RefreshServerListButton)
    {
        RefreshServerListButton->OnClicked.AddDynamic(this, &ThisClass::DisplayListOfAvailableSessions);
    }

    return true;
}

void UServerSearchList::OnFindSessions(const TArray<FOnlineSessionSearchResult> &SessionResults, bool bWasSuccessful)
{
    RefreshServerListButton->SetIsEnabled(true);
    if(bWasSuccessful == false || MultiplayerSessionsSubsystem == nullptr || SessionResults.Num() == 0) 
    {
        GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString::Printf(TEXT("Sessions searched failed search list callback!")));
        return;
    }

    for(auto SessionResult : SessionResults)
    {
        UServerSearchResult* ServerSearchResult = CreateWidget<UServerSearchResult>(GetWorld(), ServerSearchResultWidget);
        ServerSearchResult->MenuInit(SessionResult);
        if(ServerListScrollbox) ServerListScrollbox->AddChild(ServerSearchResult);
    }

    MultiplayerSessionsSubsystem->MultiplayerOnFindSessionsComplete.RemoveAll(this);
    return;
}

void UServerSearchList::BackButtonClicked()
{
    OnBackButtonServerSearchListClicked.Broadcast();
}

void UServerSearchList::DisplayListOfAvailableSessions()
{
    if(ServerListScrollbox) ServerListScrollbox->ClearChildren();

    if(RefreshServerListButton)
    {
        RefreshServerListButton->SetIsEnabled(false);
    }
    if(MultiplayerSessionsSubsystem)
    {
        MultiplayerSessionsSubsystem->FindSessions(100);
    }
}
