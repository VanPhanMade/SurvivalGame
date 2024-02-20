// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ServerSearchList.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnBackButtonClicked);
/**
 * 
 */
UCLASS()
class MULTIPLAYERTEMPLATE_API UServerSearchList : public UUserWidget
{
	GENERATED_BODY()

public:
	FOnBackButtonClicked OnBackButtonServerSearchListClicked;

	void MenuInit();

	void MenuTearDown();

	UFUNCTION()
	void DisplayListOfAvailableSessions();

protected:
	virtual bool Initialize() override; 

	void OnFindSessions(const TArray<FOnlineSessionSearchResult>& SessionResults, bool bWasSuccessful);

private:
	UPROPERTY( meta = (BindWidget))
	class UButton* RefreshServerListButton;

	UPROPERTY( meta = (BindWidget))
	class UScrollBox* ServerListScrollbox;

	UPROPERTY( meta = (BindWidget))
	class UButton* BackButton;

	UPROPERTY(EditAnywhere, Category = HUD, meta=(AllowPrivateAccess="true"))
	TSubclassOf<class UUserWidget> ServerSearchResultWidget;

	class UMultiplayerSessionsSubsystem* MultiplayerSessionsSubsystem;

	UFUNCTION()
	void BackButtonClicked();
	
};
