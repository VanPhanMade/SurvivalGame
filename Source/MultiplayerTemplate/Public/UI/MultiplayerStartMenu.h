// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "MultiplayerStartMenu.generated.h"


/**
 * 
 */
UCLASS()
class MULTIPLAYERTEMPLATE_API UMultiplayerStartMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void MenuInit(int32 NumberOfPublicConnections = 4, FString TypeOfMatch = FString(TEXT("FreeForAll")));

protected:

	virtual bool Initialize() override; 
	virtual void NativeDestruct() override;

	// Callback functions to attach to multiplayer sessions subsystem class
	// These must be UFUNCTIONs since we're using the delegate macro provided by Unreal
	UFUNCTION()
	void OnCreateSession(bool bWasSuccessful);
	UFUNCTION()
	void OnDestroySession(bool bWasSuccessful);
	UFUNCTION()
	void OnStartSession(bool bWasSuccessful);

	// These callbacks don't need UFUNCTION macro since they're not dynamic delegates 
	// in the multiplayer session
	void OnFindSessions(const TArray<FOnlineSessionSearchResult>& SessionResults, bool bWasSuccessful);
	void OnJoinSession(EOnJoinSessionCompleteResult::Type Result);

private:

	UPROPERTY( meta = (BindWidget))
	class UButton* HostButton;

	UPROPERTY( meta = (BindWidget))
	class UButton* JoinButton;

	UPROPERTY( meta = (BindWidget))
	class UButton* OptionsButton;

	UPROPERTY( meta = (BindWidget))
	class UButton* QuitButton;

	UPROPERTY(EditAnywhere, Category = HUD, meta=(AllowPrivateAccess="true"))
	TSubclassOf<class UUserWidget> OptionsMenuWidget;

	// Button on click call back functions
	UFUNCTION()
	void HostButtonClicked();

	UFUNCTION()
	void JoinButtonClicked();

	UFUNCTION()
	void OptionsButtonClicked();

	UFUNCTION()
	void QuitButtonClicked();

	// Subsystem that contains all session handling logic
	class UMultiplayerSessionsSubsystem* MultiplayerSessionsSubsystem;

	// Removes the widget when joining game session
	void MenuTearDown();

	int32 NumberPublicConnections{6};
	FString MatchType{TEXT("FreeForAll")};
};
