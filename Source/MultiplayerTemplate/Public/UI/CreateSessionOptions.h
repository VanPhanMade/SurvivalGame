// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CreateSessionOptions.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnBackButtonClicked);

/**
 * 
 */
UCLASS()
class MULTIPLAYERTEMPLATE_API UCreateSessionOptions : public UUserWidget
{
	GENERATED_BODY()

public:
	void MenuInit();

	void MenuTearDown();

	FOnBackButtonClicked OnBackButtonCreateSessionClicked;
protected:
	virtual bool Initialize() override; 

	// Callbacks for our multiplayer session events
	UFUNCTION()
	void OnCreateSession(bool bWasSuccessful);

private:
	UPROPERTY( meta = (BindWidget))
	class UEditableTextBox* MaxPlayerAmountTextBox;

	UPROPERTY( meta = (BindWidget))
	class UCheckBox* EnableLanCheckbox;

	UPROPERTY( meta = (BindWidget))
	class UButton* CreateSessionButton;

	UPROPERTY( meta = (BindWidget))
	class UButton* BackButton;

	// Callback functions for our widgets
	UFUNCTION()
	void BackButtonClicked();

	UFUNCTION()
	void CreateSessionButtonClicked();

	// Subsystem that contains all session handling logic
	class UMultiplayerSessionsSubsystem* MultiplayerSessionsSubsystem;

};
