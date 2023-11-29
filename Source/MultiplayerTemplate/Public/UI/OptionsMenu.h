// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GenericPlatform/GenericWindow.h"
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

	// ** Scalability/Graphics Level ** //
	UPROPERTY( meta = (BindWidget))
	class UTextBlock* ScalabilityLevelText;

	UPROPERTY( meta = (BindWidget))
	class UButton* ScalabilityLevelDownButton;

	UPROPERTY( meta = (BindWidget))
	class UButton* ScalabilityLevelUpButton;

	UPROPERTY(EditAnywhere, Category = HUD, meta=(AllowPrivateAccess="true"))
	TSubclassOf<class UUserWidget> StartMenuWidget;

	int32 CurrentScalabilityLevel;


	// ** Window mode ** //
	UPROPERTY( meta = (BindWidget))
	class UTextBlock* WindowModeText;

	UPROPERTY( meta = (BindWidget))
	class UButton* WindowModeDownButton;

	UPROPERTY( meta = (BindWidget))
	class UButton* WindowModeUpButton;

	int32 CurrentWindowModeIndex;
	EWindowMode::Type CurrentWindowMode;

	// ** Screen Resolution **//
	UPROPERTY( meta = (BindWidget))
	class UTextBlock* ScreenResolutionText;

	UPROPERTY( meta = (BindWidget))
	class UButton* ScreenResolutionDownButton;

	UPROPERTY( meta = (BindWidget))
	class UButton* ScreenResolutionUpButton;

	int32 CurrentScreenResolutionIndex;
	FIntPoint CurrentScreenResolution;

	// ** Audio components ** //
	UPROPERTY( meta = (BindWidget))
	class USlider* MasterAudioSlider;

	UPROPERTY( meta = (BindWidget))
	class USlider* AmbienceAudioSlider;

	UPROPERTY( meta = (BindWidget))
	class USlider* MusicAudioSlider;

	UPROPERTY( meta = (BindWidget))
	class USlider* SFXAudioSlider;

	UPROPERTY( meta = (BindWidget))
	class USlider* DialogueAudioSlider;

	UPROPERTY(EditAnywhere, Category = "Audio", meta=(AllowPrivateAccess="true"))
	class USoundClass* MasterAudioSoundClass;

	UPROPERTY(EditAnywhere, Category = "Audio", meta=(AllowPrivateAccess="true"))
	class USoundMix* MasterAudioMixer;

	UPROPERTY(EditAnywhere, Category = "Audio", meta=(AllowPrivateAccess="true"))
	class USoundClass* AmbienceAudioSoundClass;

	UPROPERTY(EditAnywhere, Category = "Audio", meta=(AllowPrivateAccess="true"))
	class USoundMix* AmbienceAudioMixer;

	UPROPERTY(EditAnywhere, Category = "Audio", meta=(AllowPrivateAccess="true"))
	class USoundClass* MusicAudioSoundClass;

	UPROPERTY(EditAnywhere, Category = "Audio", meta=(AllowPrivateAccess="true"))
	class USoundMix* MusicAudioMixer;

	UPROPERTY(EditAnywhere, Category = "Audio", meta=(AllowPrivateAccess="true"))
	class USoundClass* SFXAudioSoundClass;

	UPROPERTY(EditAnywhere, Category = "Audio", meta=(AllowPrivateAccess="true"))
	class USoundMix* SFXAudioMixer;

	UPROPERTY(EditAnywhere, Category = "Audio", meta=(AllowPrivateAccess="true"))
	class USoundClass* DialogueAudioSoundClass;

	UPROPERTY(EditAnywhere, Category = "Audio", meta=(AllowPrivateAccess="true"))
	class USoundMix* DialogueAudioMixer;

	// ** On click callbacks ** //
	UFUNCTION()
	void ReturnButtonClicked();

	UFUNCTION()
	void ApplyButtonClicked();

	UFUNCTION()
	void IncreaseScalability();

	UFUNCTION()
	void DecreaseScalability();

	UFUNCTION()
	void WindowModeDown();

	UFUNCTION()
	void WindowModeUp();

	UFUNCTION()
	void ScreenResolutionDown();

	UFUNCTION()
	void ScreenResolutionUp();

	// ** Slider value change callbacks ** //
	UFUNCTION()
	void MasterAudioSliderChange(float SliderValue);

	UFUNCTION()
	void AmbienceAudioSliderChange(float SliderValue);

	UFUNCTION()
	void MusicAudioSliderChange(float SliderValue);

	UFUNCTION()
	void SFXAudioSliderChange(float SliderValue);

	UFUNCTION()
	void DialogueAudioSliderChange(float SliderValue);

	// ** Helper functions ** //
	void SetScalabilityLevelText();

	void SetWindowModeText();

	void SetScreenResolutionText();

	void ApplyButtonCallbacks();

	
};
