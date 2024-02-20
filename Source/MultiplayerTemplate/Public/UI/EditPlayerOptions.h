// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EditPlayerOptions.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnBackButtonEditPlayerClicked);

/**
 * 
 */
UCLASS()
class MULTIPLAYERTEMPLATE_API UEditPlayerOptions : public UUserWidget
{
	GENERATED_BODY()

public:
	void MenuInit();

	void RefreshOptions();

	FOnBackButtonEditPlayerClicked OnBackButtonEditPlayerClicked;
protected:
	virtual bool Initialize() override; 

private:
	UPROPERTY( meta = (BindWidget))
	class UButton* BackButton;

	UPROPERTY( meta = (BindWidget))
	class UScrollBox* CharacterOptionsScrollbox;

	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess="true"))
	class UDataTable* CharactersDataTable;

	UPROPERTY(EditAnywhere, Category = HUD, meta=(AllowPrivateAccess="true"))
	TSubclassOf<class UUserWidget> PlayerOptionWidget;

	UFUNCTION()
	void BackButtonClicked();
	
};
