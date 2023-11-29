// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/OptionsMenu.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Slider.h"
#include "UI/MultiplayerStartMenu.h"
#include "GameFramework/GameUserSettings.h"
#include "Math/UnrealMathUtility.h"
#include "Sound/SoundClass.h"
#include "Sound/SoundMix.h"

void UOptionsMenu::MenuInit()
{
    AddToViewport();
    SetVisibility(ESlateVisibility::Visible);
    bIsFocusable = true;
    
    // When the UI is initialized on the screen, show the cursor 
    UWorld* World = GetWorld();
    if(World)
    {
        APlayerController* PlayerController = World->GetFirstPlayerController();
        if(PlayerController)
        {
            FInputModeUIOnly InputModeData;
            InputModeData.SetWidgetToFocus(TakeWidget());
            InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
            PlayerController->SetInputMode(InputModeData);
            PlayerController->SetShowMouseCursor(true);
        }
    }

    ApplyButtonCallbacks();

    UGameUserSettings* UserSettings = UGameUserSettings::GetGameUserSettings();
    CurrentScalabilityLevel = UserSettings->GetOverallScalabilityLevel();
    SetScalabilityLevelText();

    CurrentWindowMode = UserSettings->GetFullscreenMode();
    switch (CurrentWindowMode)
    {
        case EWindowMode::Fullscreen:
            CurrentWindowModeIndex = 0;
            break;

        case EWindowMode::WindowedFullscreen:
            CurrentWindowModeIndex = 1;
            break;

        case EWindowMode::Windowed:
            CurrentWindowModeIndex = 2;
            break;

        default:
            CurrentWindowModeIndex = 0;
            break;  
    }

    SetWindowModeText();

    CurrentScreenResolution = UserSettings->GetScreenResolution();
    FString XResolution = FString::FromInt(CurrentScreenResolution.X);
    FString YResolution = FString::FromInt(CurrentScreenResolution.Y);
    FText CurrentScreenResolutionText = FText::FromString(XResolution + " x " + YResolution);
    ScreenResolutionText->SetText(CurrentScreenResolutionText);
    // Calculating approximate index 
    if(CurrentScreenResolution.Size() == FIntPoint(1920, 1080).Size()) 
    {
        CurrentScreenResolutionIndex = 9;
    }
    else
    {
        if(CurrentScreenResolution.Size() > FIntPoint(1920, 1080).Size())
        {
            CurrentScreenResolutionIndex = 10;
            if(CurrentScreenResolution.Size() > FIntPoint(2560, 1440).Size())
            {
                CurrentScreenResolutionIndex = 11;
                if(CurrentScreenResolution.Size() > FIntPoint(2560, 1600).Size())
                {
                    CurrentScreenResolutionIndex = 12;
                    if(CurrentScreenResolution.Size() > FIntPoint(3840, 2160).Size())
                    {
                        CurrentScreenResolutionIndex = 13; // Assumed all sizes 7680 x 4320 largest
                    }
                }
            }
        }
        else
        {
            CurrentScreenResolutionIndex = 8;
            if(CurrentScreenResolution.Size() < FIntPoint(1680, 1050).Size())
            {
                CurrentScreenResolutionIndex = 7;
                if(CurrentScreenResolution.Size() < FIntPoint(1600, 900).Size())
                {
                    CurrentScreenResolutionIndex = 6;
                    if(CurrentScreenResolution.Size() < FIntPoint(1536, 864).Size())
                    {
                        CurrentScreenResolutionIndex = 5;
                        if(CurrentScreenResolution.Size() < FIntPoint(1280, 1024).Size())
                        {
                            CurrentScreenResolutionIndex = 4;
                            if(CurrentScreenResolution.Size() < FIntPoint(1440, 900).Size())
                            {
                                CurrentScreenResolutionIndex = 3;
                                if(CurrentScreenResolution.Size() < FIntPoint(1366, 768).Size())
                                {
                                    CurrentScreenResolutionIndex = 2;
                                    if(CurrentScreenResolution.Size() < FIntPoint(1280, 800).Size())
                                    {
                                        CurrentScreenResolutionIndex = 1;
                                        if(CurrentScreenResolution.Size() < FIntPoint(1280, 720).Size())
                                        {
                                            CurrentScreenResolutionIndex = 0; // Assumed all sizes 1024 x 768 smallest
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    
    if(MasterAudioSoundClass && MasterAudioSlider) MasterAudioSlider->SetValue(MasterAudioSoundClass->Properties.Volume);
    if(AmbienceAudioSoundClass && AmbienceAudioSlider) AmbienceAudioSlider->SetValue(AmbienceAudioSoundClass->Properties.Volume);
    if(MusicAudioSoundClass && MusicAudioSlider) MusicAudioSlider->SetValue(MusicAudioSoundClass->Properties.Volume);
    if(SFXAudioSoundClass && SFXAudioSlider) SFXAudioSlider->SetValue(SFXAudioSoundClass->Properties.Volume);
    if(DialogueAudioSoundClass && DialogueAudioSlider) DialogueAudioSlider->SetValue(DialogueAudioSoundClass->Properties.Volume);
}

bool UOptionsMenu::Initialize()
{
    if(!Super::Initialize()) return false;

    ApplyButtonCallbacks();

    return true;
}

void UOptionsMenu::ReturnButtonClicked()
{
    if(!IsValid(StartMenuWidget)) return;
    
    UMultiplayerStartMenu* StartMenu = CreateWidget<UMultiplayerStartMenu>(GetWorld(), StartMenuWidget);
    StartMenu->MenuInit();
    RemoveFromParent();
}

void UOptionsMenu::ApplyButtonClicked()
{
    UGameUserSettings* UserSettings = UGameUserSettings::GetGameUserSettings();
    UserSettings->SetOverallScalabilityLevel(CurrentScalabilityLevel);
    UserSettings->SetFullscreenMode(CurrentWindowMode);
    UserSettings->SetScreenResolution(CurrentScreenResolution);
    UserSettings->RequestResolutionChange(CurrentScreenResolution.X, CurrentScreenResolution.Y, CurrentWindowMode, false);
    UserSettings->ApplyResolutionSettings(false);
    UserSettings->ApplySettings(false);
}

void UOptionsMenu::IncreaseScalability()
{
    CurrentScalabilityLevel++;
    CurrentScalabilityLevel = FMath::Clamp(CurrentScalabilityLevel, 0, 4);
    SetScalabilityLevelText();
}

void UOptionsMenu::DecreaseScalability()
{
    CurrentScalabilityLevel--;
    CurrentScalabilityLevel = FMath::Clamp(CurrentScalabilityLevel, 0, 4);
    SetScalabilityLevelText();
}

void UOptionsMenu::WindowModeDown()
{
    CurrentWindowModeIndex--;
    CurrentWindowModeIndex = FMath::Clamp(CurrentWindowModeIndex, 0, 2);
    SetWindowModeText();
}

void UOptionsMenu::WindowModeUp()
{
    CurrentWindowModeIndex++;
    CurrentWindowModeIndex = FMath::Clamp(CurrentWindowModeIndex, 0, 2);
    SetWindowModeText();
}

void UOptionsMenu::ScreenResolutionDown()
{
    CurrentScreenResolutionIndex--;
    CurrentScreenResolutionIndex = FMath::Clamp(CurrentScreenResolutionIndex, 0, 13);
    SetScreenResolutionText();
}

void UOptionsMenu::ScreenResolutionUp()
{
    CurrentScreenResolutionIndex++;
    CurrentScreenResolutionIndex = FMath::Clamp(CurrentScreenResolutionIndex, 0, 13);
    SetScreenResolutionText();
}

void UOptionsMenu::MasterAudioSliderChange(float SliderValue)
{
    float NewVolume = FMath::Clamp(SliderValue, .001, 1);
    MasterAudioSoundClass->Properties.Volume = NewVolume;
}

void UOptionsMenu::AmbienceAudioSliderChange(float SliderValue)
{
    float NewVolume = FMath::Clamp(SliderValue, .001, 1);
    AmbienceAudioSoundClass->Properties.Volume = NewVolume;
}

void UOptionsMenu::MusicAudioSliderChange(float SliderValue)
{
    float NewVolume = FMath::Clamp(SliderValue, .001, 1);
    MusicAudioSoundClass->Properties.Volume = NewVolume;
}

void UOptionsMenu::SFXAudioSliderChange(float SliderValue)
{
    float NewVolume = FMath::Clamp(SliderValue, .001, 1);
    SFXAudioSoundClass->Properties.Volume = NewVolume;
}

void UOptionsMenu::DialogueAudioSliderChange(float SliderValue)
{
    float NewVolume = FMath::Clamp(SliderValue, .001, 1);
    DialogueAudioSoundClass->Properties.Volume = NewVolume;
}

void UOptionsMenu::SetScalabilityLevelText()
{
    switch (CurrentScalabilityLevel)
    {
        case 0:
            ScalabilityLevelText->SetText(FText::FromString("Low"));
            break;
        case 1:
            ScalabilityLevelText->SetText(FText::FromString("Medium"));
            break;
        case 2:
            ScalabilityLevelText->SetText(FText::FromString("High"));
            break;
        case 3:
            ScalabilityLevelText->SetText(FText::FromString("Epic"));
            break;
        case 4:
            ScalabilityLevelText->SetText(FText::FromString("Cinematic"));
            break;
        case -1:
            ScalabilityLevelText->SetText(FText::FromString("Custom"));
            break;
        default:
            ScalabilityLevelText->SetText(FText::FromString("Custom"));
            break;
    }
}

void UOptionsMenu::SetWindowModeText()
{
    switch (CurrentWindowModeIndex)
    {
        case 0:
            WindowModeText->SetText(FText::FromString("Fullscreen"));
            CurrentWindowMode = EWindowMode::Fullscreen;
            break;
        case 1:
            WindowModeText->SetText(FText::FromString("Windowed Fullscreen"));
            CurrentWindowMode = EWindowMode::WindowedFullscreen;
            break;
        case 2:
            WindowModeText->SetText(FText::FromString("Windowed"));
            CurrentWindowMode = EWindowMode::Windowed;
            break;

        default:
            WindowModeText->SetText(FText::FromString("Custom"));
            break;
    }
}

void UOptionsMenu::SetScreenResolutionText()
{
    switch (CurrentScreenResolutionIndex)
    {
    case 13:
        ScreenResolutionText->SetText(FText::FromString("7680 x 4320"));
        CurrentScreenResolution = FIntPoint(7680, 4230);
        break;
    case 12:
        ScreenResolutionText->SetText(FText::FromString("3840 x 2160"));
        CurrentScreenResolution = FIntPoint(3840, 2160);
        break;
    case 11:
        ScreenResolutionText->SetText(FText::FromString("2560 x 1600"));
        CurrentScreenResolution = FIntPoint(2560, 1600);
        break;
    case 10:
        ScreenResolutionText->SetText(FText::FromString("2560 x 1440"));
        CurrentScreenResolution = FIntPoint(2560, 1440);
        break;
    case 9:
        ScreenResolutionText->SetText(FText::FromString("1920 x 1080"));
        CurrentScreenResolution = FIntPoint(1920, 1080);
        break;
    case 8:
        ScreenResolutionText->SetText(FText::FromString("1680 x 1050"));
        CurrentScreenResolution = FIntPoint(1680, 1050);
        break;
    case 7:
        ScreenResolutionText->SetText(FText::FromString("1600 x 900"));
        CurrentScreenResolution = FIntPoint(1600, 900);
        break;
    case 6:
        ScreenResolutionText->SetText(FText::FromString("1536 x 864"));
        CurrentScreenResolution = FIntPoint(1536, 864);
        break;
    case 5:
        ScreenResolutionText->SetText(FText::FromString("1280 x 1024"));
        CurrentScreenResolution = FIntPoint(1280, 1024);
        break;
    case 4:
        ScreenResolutionText->SetText(FText::FromString("1440 x 900"));
        CurrentScreenResolution = FIntPoint(1440, 900);
        break;
    case 3:
        ScreenResolutionText->SetText(FText::FromString("1366 x 768"));
        CurrentScreenResolution = FIntPoint(1366, 768);
        break;
    case 2:
        ScreenResolutionText->SetText(FText::FromString("1280 x 800"));
        CurrentScreenResolution = FIntPoint(1280, 800);
        break;
    case 1:
        ScreenResolutionText->SetText(FText::FromString("1280 x 720"));
        CurrentScreenResolution = FIntPoint(1280, 720);
        break;
    case 0:
        ScreenResolutionText->SetText(FText::FromString("1024 x 768"));
        CurrentScreenResolution = FIntPoint(1024, 768);
        break;
    
    default:
        ScreenResolutionText->SetText(FText::FromString("Custom"));
        break;
    }
}

void UOptionsMenu::ApplyButtonCallbacks()
{
    if(ReturnButton && !ReturnButton->OnClicked.IsBound())
    {
        ReturnButton->OnClicked.AddDynamic(this, &ThisClass::ReturnButtonClicked);
    }

    if(ApplyButton && !ApplyButton->OnClicked.IsBound())
    {
        ApplyButton->OnClicked.AddDynamic(this, &ThisClass::ApplyButtonClicked);
    }

    if(ScalabilityLevelUpButton && !ScalabilityLevelUpButton->OnClicked.IsBound())
    {
        ScalabilityLevelUpButton->OnClicked.AddDynamic(this, &ThisClass::IncreaseScalability);
    }

    if(ScalabilityLevelDownButton && !ScalabilityLevelDownButton->OnClicked.IsBound())
    {
        ScalabilityLevelDownButton->OnClicked.AddDynamic(this, &ThisClass::DecreaseScalability);
    }

    if(WindowModeDownButton && !WindowModeDownButton->OnClicked.IsBound())
    {
        WindowModeDownButton->OnClicked.AddDynamic(this, &ThisClass::WindowModeDown);
    }

    if(WindowModeUpButton && !WindowModeUpButton->OnClicked.IsBound())
    {
        WindowModeUpButton->OnClicked.AddDynamic(this, &ThisClass::WindowModeUp);
    }

    if(ScreenResolutionDownButton && !ScreenResolutionDownButton->OnClicked.IsBound())
    {
        ScreenResolutionDownButton->OnClicked.AddDynamic(this, &ThisClass::ScreenResolutionDown);
    }

    if(ScreenResolutionUpButton && !ScreenResolutionUpButton->OnClicked.IsBound())
    {
        ScreenResolutionUpButton->OnClicked.AddDynamic(this, &ThisClass::ScreenResolutionUp);
    }

    if(MasterAudioSlider && !MasterAudioSlider->OnValueChanged.IsBound())
    {
        MasterAudioSlider->OnValueChanged.AddDynamic(this, &ThisClass::MasterAudioSliderChange);
    }

    if(AmbienceAudioSlider && !AmbienceAudioSlider->OnValueChanged.IsBound())
    {
        AmbienceAudioSlider->OnValueChanged.AddDynamic(this, &ThisClass::AmbienceAudioSliderChange);
    }

    if(MusicAudioSlider && !MusicAudioSlider->OnValueChanged.IsBound())
    {
        MusicAudioSlider->OnValueChanged.AddDynamic(this, &ThisClass::MusicAudioSliderChange);
    }

    if(SFXAudioSlider && !SFXAudioSlider->OnValueChanged.IsBound())
    {
        SFXAudioSlider->OnValueChanged.AddDynamic(this, &ThisClass::SFXAudioSliderChange);
    }

    if(DialogueAudioSlider && !DialogueAudioSlider->OnValueChanged.IsBound())
    {
        DialogueAudioSlider->OnValueChanged.AddDynamic(this, &ThisClass::DialogueAudioSliderChange);
    }
}
