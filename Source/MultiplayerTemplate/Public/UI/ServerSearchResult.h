// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ServerSearchResult.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERTEMPLATE_API UServerSearchResult : public UUserWidget
{
	GENERATED_BODY()

public:

protected:
	virtual bool Initialize() override; 

private:
	UPROPERTY( meta = (BindWidget))
	class UTextBlock* SessionNameText;

	UPROPERTY( meta = (BindWidget))
	class UTextBlock* PlayerAmountText;

	UPROPERTY( meta = (BindWidget))
	class UTextBlock* PingText;

	UPROPERTY( meta = (BindWidget))
	class UButton* JoinServerButton;
};
