// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OptionsMenu.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERTEMPLATE_API UOptionsMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	void MenuInit();

protected:
	virtual bool Initialize() override;

private:
	UPROPERTY( meta = (BindWidget))
	class UButton* ReturnButton;

	UPROPERTY( meta = (BindWidget))
	class UButton* ApplyButton;

	UPROPERTY(EditAnywhere, Category = HUD, meta=(AllowPrivateAccess="true"))
	TSubclassOf<class UUserWidget> StartMenuWidget;

	UFUNCTION()
	void ReturnButtonClicked();

	UFUNCTION()
	void ApplyButtonClicked();



	
};
