#include "Characters/BasicCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "GameInstanceSubsystems/MultiplayerSessionsSubsystem.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "UI/MultiplayerInGameMenu.h"

#include "Sound/SoundBase.h"


ABasicCharacter::ABasicCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Spring Arm"));
	SpringArmComponent->SetupAttachment(GetCapsuleComponent());
	SpringArmComponent->bUsePawnControlRotation = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Player Camera"));
	CameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);
	CameraComponent->bUsePawnControlRotation = false;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

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
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ThisClass::Look);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

	}
}

void ABasicCharacter::Interact() 
{
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

void ABasicCharacter::Move(const FInputActionValue &Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ABasicCharacter::Look(const FInputActionValue &Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(-LookAxisVector.Y);
	}
}
