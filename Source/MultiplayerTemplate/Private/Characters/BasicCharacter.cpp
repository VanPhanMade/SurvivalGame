#include "Characters/BasicCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "GameInstanceSubsystems/MultiplayerSessionsSubsystem.h"

#include "Camera/CameraComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "UI/MultiplayerInGameMenu.h"

#include "Sound/SoundBase.h"


ABasicCharacter::ABasicCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ABasicCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void ABasicCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABasicCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &ThisClass::Interact);
		EnhancedInputComponent->BindAction(StartSessionAction, ETriggerEvent::Triggered, this, &ThisClass::StartSession);
		EnhancedInputComponent->BindAction(OpenInGameMenuAction, ETriggerEvent::Triggered, this, &ThisClass::OpenInGameMenu);
	}
}

/** Callback for interact input action */
void ABasicCharacter::Interact() 
{
	/** Example of 2D axis map */
	//void ABasicCharacter::Move(const FInputActionValue &Value)
	UE_LOG(LogTemp, Display, TEXT("Interact"));

	if(InteractSFX) UGameplayStatics::SpawnSoundAtLocation(this, InteractSFX, GetActorLocation());
}

void ABasicCharacter::StartSession()
{
	UWorld* World = GetWorld();
	if(World)
	{
		if(GetLocalRole() == ENetRole::ROLE_Authority)
		{
			if(GEngine)
			{
				GEngine->AddOnScreenDebugMessage(
					2,
					15.f,
					FColor::Yellow,
					FString::Printf(TEXT("Starting server session..."))
				);
			}

			UGameInstance* GameInstance = GetGameInstance();
    		if(GameInstance)
    		{
    		    UMultiplayerSessionsSubsystem* MultiplayerSessionsSubsystem = GameInstance->GetSubsystem<UMultiplayerSessionsSubsystem>();
				MultiplayerSessionsSubsystem->StartSession();
    		}
		}
	}
}

void ABasicCharacter::OpenInGameMenu()
{
	if(!IsValid(InGameMenuWidget)) return;

	if(MultiplayerInGameMenu == nullptr) 
	{
		MultiplayerInGameMenu = Cast<UMultiplayerInGameMenu>(CreateWidget<UMultiplayerInGameMenu>(GetWorld(), InGameMenuWidget));
	}

	if(IsValid(MultiplayerInGameMenu))
	{
		bInGameMenuOpen = !bInGameMenuOpen;
		if(bInGameMenuOpen)
		{
			MultiplayerInGameMenu->MenuInit();
		}
		else
		{
			MultiplayerInGameMenu->MenuTearDown();
		}
	}
}
