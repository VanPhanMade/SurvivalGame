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

protected:
	virtual bool Initialize() override; 

private:
	UPROPERTY( meta = (BindWidget))
	class UButton* RefreshServerListButton;

	UPROPERTY( meta = (BindWidget))
	class UScrollBox* ServerListScrollbox;

	UPROPERTY( meta = (BindWidget))
	class UButton* BackButton;

	UFUNCTION()
	void BackButtonClicked();
	
};
