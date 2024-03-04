#include "Characters/BasicCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "GameInstanceSubsystems/MultiplayerSessionsSubsystem.h"
#include "Net/UnrealNetwork.h"
#include "DrawDebugHelpers.h"
#include "TimerManager.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Actors/Interactable.h"
#include "Components/CapsuleComponent.h"
#include "GameModes/MainLevelGameMode.h"
#include "Components/SkeletalMeshComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "UI/MultiplayerInGameMenu.h"
#include "DataTables/CharacterPresetsDataTable.h"
#include "SaveGame/MultiplayerSaveGameData.h"

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
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
	
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	SetReplicates(true);
	SetReplicateMovement(true);
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

	Server_SetDead(false);

	// if(IsLocallyControlled())
	// {
	// 	UMultiplayerSaveGameData* SaveData = Cast<UMultiplayerSaveGameData>(UGameplayStatics::CreateSaveGameObject(UMultiplayerSaveGameData::StaticClass()));
	// 	SaveData = Cast<UMultiplayerSaveGameData>(UGameplayStatics::LoadGameFromSlot("CharacterSelection", 0));
	// 	if(SaveData)
	// 	{
	// 		LoadCharacterSelection();
	// 		Server_SetLoadForCharacter(GetMesh()->SkeletalMesh);
	// 	}
	// }
	
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
		EnhancedInputComponent->BindAction(StartSessionAction, ETriggerEvent::Triggered, this, &ThisClass::StartSession);
		EnhancedInputComponent->BindAction(OpenInGameMenuAction, ETriggerEvent::Triggered, this, &ThisClass::OpenInGameMenu);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ThisClass::Look);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ThisClass::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ThisClass::StopJumping);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &ThisClass::Interact);
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Triggered, this, &ThisClass::Crouch);
		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Triggered, this, &ThisClass::Aim);
		EnhancedInputComponent->BindAction(AimActionReleased, ETriggerEvent::Triggered, this, &ThisClass::AimReleased);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &ThisClass::Attack);
	}
}

void ABasicCharacter::GetLifetimeReplicatedProps(TArray < FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION(ABasicCharacter, bCanFight, COND_None);
	DOREPLIFETIME_CONDITION(ABasicCharacter, bAiming, COND_None);
	DOREPLIFETIME_CONDITION(ABasicCharacter, ForwardScalar, COND_None);
	DOREPLIFETIME_CONDITION(ABasicCharacter, RightScalar, COND_None);
	DOREPLIFETIME_CONDITION(ABasicCharacter, CurrentHealth, COND_None);
	DOREPLIFETIME_CONDITION(ABasicCharacter, bIsDead, COND_None);
	DOREPLIFETIME(ABasicCharacter, UpdatedMesh);
}

void ABasicCharacter::StartSession()
{
	UWorld* World = GetWorld();
	if(World)
	{
		if(GetLocalRole() == ENetRole::ROLE_Authority)
		{
			if(GEngine) GEngine->AddOnScreenDebugMessage(2, 15.f, FColor::Yellow, FString::Printf(TEXT("Starting server session...")));

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
	
		RightScalar = FMath::FInterpTo(RightScalar, MovementVector.X, UGameplayStatics::GetWorldDeltaSeconds(GetWorld()), 6.f);
		ForwardScalar = FMath::FInterpTo(ForwardScalar, MovementVector.Y, UGameplayStatics::GetWorldDeltaSeconds(GetWorld()), 6.f);
		Server_SetInputXY(MovementVector.X, MovementVector.Y);
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

void ABasicCharacter::Interact()
{
	FVector2D ViewportSize;
	if(GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);
	}

	FVector2D CrosshairLocation(ViewportSize.X / 2.f, ViewportSize.Y / 2.f);
	FVector CrosshairWorldPosition;
	FVector CrosshairWorldDirection;
	
	if(UGameplayStatics::DeprojectScreenToWorld(UGameplayStatics::GetPlayerController(this, 0), CrosshairLocation, CrosshairWorldPosition, CrosshairWorldDirection))
	{
		FVector Start = CrosshairWorldPosition;
		FVector End = Start + (CrosshairWorldDirection * InteractionMaxDistance);
		FHitResult HitResult;
		FCollisionShape Sphere = FCollisionShape::MakeSphere(32);
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(this);	
		GetWorld()->SweepSingleByChannel(HitResult, Start, End, FQuat::Identity, ECollisionChannel::ECC_Visibility, Sphere, QueryParams);

		if(!HitResult.bBlockingHit)
		{
			HitResult.ImpactPoint = End;
		}
		else
		{
			AInteractable* HitInteractable = Cast<AInteractable>(HitResult.GetActor());
			if(HitInteractable)
			{
				Server_DestroyActor(HitInteractable);
				Server_SetFighting(true);
			}
		}
	}
}

void ABasicCharacter::Crouch()
{
	if(bIsCrouched) ACharacter::UnCrouch();
	else ACharacter::Crouch();
}

void ABasicCharacter::Aim()
{
	bAiming = true;
	Server_SetAiming(true);
}

void ABasicCharacter::AimReleased()
{
	bAiming = false;
	Server_SetAiming(false);
}

void ABasicCharacter::Attack()
{
	if(!bCanFight) return;
	Server_Attack();
}

void ABasicCharacter::Server_DestroyActor_Implementation(AActor *ActorToDestroy)
{
	if(IsValid(ActorToDestroy))
	{
		if(Cast<AInteractable>(ActorToDestroy))
		{
			if(PickupItemMontage) Multicast_PlayAnim(PickupItemMontage, this);
		}
		Multicast_DestroyActor(ActorToDestroy);
	}
}

void ABasicCharacter::Multicast_DestroyActor_Implementation(AActor *ActorToDestroy)
{
	if(IsValid(ActorToDestroy))
	{
		ActorToDestroy->Destroy();
	}
}

void ABasicCharacter::Server_SetAiming_Implementation(bool bIsAiming)
{
	bAiming = bIsAiming;
}

void ABasicCharacter::Server_SetFighting_Implementation(bool bIsFight)
{
	bCanFight = bIsFight;
}

void ABasicCharacter::Server_SetLoadForCharacter_Implementation(USkeletalMesh* NewMeshComponent)
{
	if(NewMeshComponent)
	{
		UpdatedMesh = NewMeshComponent;
		GetMesh()->SetSkeletalMesh(NewMeshComponent, true);
		Multicast_SetLoadForCharacter(NewMeshComponent, this);
	}
}

void ABasicCharacter::OnRep_CurrentHealth()
{
	// if(CurrentHealth == MaxHealth) DrawDebugSphere(GetWorld(), GetActorLocation(), 32, 16, FColor::Green, true);
	// else if(CurrentHealth == 0) DrawDebugSphere(GetWorld(), GetActorLocation(), 40, 16, FColor::Red, true);
	// else DrawDebugSphere(GetWorld(), GetActorLocation(), 16, 16, FColor::Yellow, true);
}

void ABasicCharacter::OnRep_UpdatedMesh()
{
	if(UpdatedMesh)
	{	
		GetMesh()->SetSkeletalMesh(UpdatedMesh, true);
	}
	
}

void ABasicCharacter::Server_DoDamage_Implementation(AActor* AttackedActor, AActor* AttackerActor)
{
	if(HitByPunchMontage)
	{
		Multicast_PlayHitByAttackAnim(HitByPunchMontage, AttackedActor);
	}
	
	ABasicCharacter* HitCharacter = Cast<ABasicCharacter>(AttackedActor);
	HitCharacter->CurrentHealth = FMath::Clamp(HitCharacter->CurrentHealth - 20.f, 0.f, MaxHealth);

	AMainLevelGameMode* GameMode = GetWorld()->GetAuthGameMode<AMainLevelGameMode>();
	if(GameMode && HitCharacter->CurrentHealth == 0.f)
	{
		HitCharacter->Server_SetDead(true);
		ABasicCharacter* AttackerCharacter = Cast<ABasicCharacter>(AttackerActor);
		GameMode->PlayerEliminated(HitCharacter, Cast<APlayerController>(HitCharacter->GetController()), Cast<APlayerController>(AttackerCharacter->GetController()));
	}
}

void ABasicCharacter::Server_Attack_Implementation()
{
	Multicast_PlayAttackAnim();

	FVector Start = GetActorLocation();
	FVector End = Start + GetActorForwardVector() * 100.f;

	FCollisionShape Sphere = FCollisionShape::MakeSphere(64);
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	FHitResult HitResult;
	if(GetWorld()->SweepSingleByChannel(HitResult, Start, End, FQuat::Identity, ECollisionChannel::ECC_Pawn, Sphere, QueryParams))
	{
		ABasicCharacter* HitCharacter = Cast<ABasicCharacter>(HitResult.GetActor());
		if(HitCharacter)
		{
			Server_DoDamage(HitCharacter, this);
		}
	}
}

void ABasicCharacter::Multicast_ApplyDeath_Implementation()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if(AnimInstance && DeathMontage)
	{
		AnimInstance->Montage_Play(DeathMontage);
	}
}

void ABasicCharacter::Server_SetDead_Implementation(bool IsDead)
{
	bIsDead = IsDead;
	if(bIsDead)
	{
		FTimerHandle RespawnTimerHandle;
		GetWorldTimerManager().SetTimer(RespawnTimerHandle, this, &ThisClass::DeathTimerFinished, RespawnTime);
	}
}

void ABasicCharacter::Server_SetInputXY_Implementation(float XInput, float YInput)
{
	RightScalar = FMath::FInterpTo(RightScalar, XInput, UGameplayStatics::GetWorldDeltaSeconds(GetWorld()), 6.f);
	ForwardScalar = FMath::FInterpTo(ForwardScalar, YInput, UGameplayStatics::GetWorldDeltaSeconds(GetWorld()), 6.f);
}

void ABasicCharacter::Multicast_PlayAttackAnim_Implementation()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if(AnimInstance && PunchMontage)
	{
		AnimInstance->Montage_Play(PunchMontage);
	}
}

void ABasicCharacter::Multicast_PlayHitByAttackAnim_Implementation(UAnimMontage* MontageToPlay, AActor* AttackedActor)
{
	if(MontageToPlay && AttackedActor)
	{
		ABasicCharacter* AttackedPlayer = Cast<ABasicCharacter>(AttackedActor);
		if(AttackedPlayer)
		{
			UAnimInstance* AnimInstance = AttackedPlayer->GetMesh()->GetAnimInstance();
			if(AnimInstance) AnimInstance->Montage_Play(MontageToPlay);
		}	
	}
}

void ABasicCharacter::Multicast_PlayAnim_Implementation(UAnimMontage* MontageToPlay, AActor* AppliedActor)
{
	if(MontageToPlay && AppliedActor)
	{
		ABasicCharacter* AppliedPlayer = Cast<ABasicCharacter>(AppliedActor);
		if(AppliedPlayer)
		{
			UAnimInstance* AnimInstance = AppliedPlayer->GetMesh()->GetAnimInstance();
			if(AnimInstance) AnimInstance->Montage_Play(MontageToPlay);
		}	
	}
}

void ABasicCharacter::Multicast_SetLoadForCharacter_Implementation(USkeletalMesh* NewMeshComponent, AActor* LoadedCharacter)
{
	if(NewMeshComponent)
	{
		if(LoadedCharacter)
		{
			Cast<ABasicCharacter>(LoadedCharacter)->GetMesh()->SetSkeletalMesh(NewMeshComponent, true);
		}
	}
}

bool ABasicCharacter::GetCanFight()
{
	return bCanFight;
}

bool ABasicCharacter::GetIsAiming()
{
    return bAiming;
}

FVector2D ABasicCharacter::GetInputXY()
{
    return FVector2D(RightScalar, ForwardScalar);
}

void ABasicCharacter::DeathTimerFinished()
{
	AMainLevelGameMode* GameMode = GetWorld()->GetAuthGameMode<AMainLevelGameMode>();
	if(GameMode)
	{
		GameMode->RequestRespawn(this, Controller);
	}
}

void ABasicCharacter::LoadCharacterSelection()
{
	USkeletalMeshComponent* CurrentMesh = GetMesh();
	if(UpdatedMesh != nullptr)
	{
		CurrentMesh->SetSkeletalMesh(UpdatedMesh, true);
	}
	
}

void ABasicCharacter::LoadCharacterSelection_Implementation(USkeletalMesh *NewMesh)
{
	if(NewMesh != nullptr)
	{
		UpdatedMesh = NewMesh;
	}

	if(UpdatedMesh)
	{
		USkeletalMeshComponent* CurrentMesh = GetMesh();
		CurrentMesh->SetSkeletalMesh(UpdatedMesh, true);
		Server_SetLoadForCharacter(UpdatedMesh);
	}
}
