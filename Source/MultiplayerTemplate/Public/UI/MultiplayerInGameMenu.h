// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MultiplayerInGameMenu.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERTEMPLATE_API UMultiplayerInGameMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	void MenuInit();

	void MenuTearDown();

protected:
	virtual bool Initialize() override;

private:
	UPROPERTY( meta = (BindWidget))
	class UButton* ExitGameButton;

	UFUNCTION()
	void ExitGameButtonClicked();

	UPROPERTY()
	class UMultiplayerSessionsSubsystem* MultiplayerSessionsSubsystem;

	UPROPERTY()
	class APlayerController* PlayerController;

};
