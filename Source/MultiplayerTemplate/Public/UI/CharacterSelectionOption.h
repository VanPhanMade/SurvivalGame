// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterSelectionOption.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERTEMPLATE_API UCharacterSelectionOption : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void MenuInit(FName PresetName, class USkeletalMesh* _CharacterModel, class UTexture2D* _CharacterImage, FText _CharacterDescription, class USoundBase* _SelectionAudio);

protected:
	virtual bool Initialize() override; 
	
private:
	UPROPERTY( meta = (BindWidget))
	class UTextBlock* CharacterName;

	UPROPERTY( meta = (BindWidget))
	class UButton* SelectButton;

	UPROPERTY( meta = (BindWidget))
	class UImage* CharacterImage;

	UPROPERTY( meta = (BindWidget))
	class UTextBlock* CharacterDescription;

	UFUNCTION()
	void SelectButtonClicked();

	class USkeletalMesh* CharacterModel;

	class USoundBase* OnClickAudio;

	

};
