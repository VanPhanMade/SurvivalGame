// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Interfaces/OnlineSessionInterface.h" //Contains type defs for session interfaces
#include "BasicCharacter.generated.h"

UCLASS()
class MULTIPLAYERTEMPLATE_API ABasicCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABasicCharacter();
	friend class AMainLevelGameMode;
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void DeathTimerFinished();

private:
	/** Input */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* StartSessionAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* OpenInGameMenuAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* InteractAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* CrouchAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* AimAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* AimActionReleased;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* AttackAction;

	/** Components */
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* SpringArmComponent;
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* CameraComponent;

	/** Variables */
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess="true"))
	class USoundBase* InteractSFX;

	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess="true"))
	TSubclassOf<class UUserWidget> InGameMenuWidget;

	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess="true"))
	class UMultiplayerInGameMenu* MultiplayerInGameMenu;

	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess="true"), meta=(ClampMin = 0))
	float InteractionMaxDistance = 200.f;
	
	UPROPERTY(VisibleAnywhere, meta=(AllowPrivateAccess="true"))
	bool bInGameMenuOpen = false;

	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess="true"), meta=(ClampMin = 0))
	float MaxHealth = 100.f;

	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess="true"), meta=(ClampMin = 0))
	float RespawnTime = 3.f;

	UPROPERTY(EditAnywhere)
	UAnimMontage* DeathMontage;

	UPROPERTY(EditAnywhere)
	UAnimMontage* PunchMontage;

	UPROPERTY(EditAnywhere)
	UAnimMontage* HitByPunchMontage;

	/** Input Callback Functions */
	void StartSession();
	void OpenInGameMenu();
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Interact();
	void Crouch();
	void Aim();
	void AimReleased();
	void Attack();

	// Replicated Variables
	UPROPERTY()
	class AInteractable* LastInteractable;

	UPROPERTY(Replicated)
	bool bIsDead = false;

	UPROPERTY(Replicated)
	bool bCanFight = false;

	UPROPERTY(Replicated)
	bool bAiming = false;

	UPROPERTY(Replicated)
	float ForwardScalar = 0.f; 

	UPROPERTY(Replicated)
	float RightScalar = 0.f;

	UPROPERTY(ReplicatedUsing = OnRep_CurrentHealth, VisibleAnywhere)
	float CurrentHealth = 100.f;

	UFUNCTION()
	void OnRep_CurrentHealth();

	// RPCs
	UFUNCTION(Server, Reliable)
	void Server_DestroyActor(AActor* ActorToDestroy);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_DestroyActor(AActor* ActorToDestroy);

	UFUNCTION(Server, Reliable)
	void Server_SetAiming(bool bIsAiming);

	UFUNCTION(Server, Reliable)
	void Server_SetFighting(bool bIsFight);

	UFUNCTION(Server, Reliable)
	void Server_DoDamage(AActor* AttackedActor, AActor* AttackerActor);

	UFUNCTION(Server, Reliable)
	void Server_Attack();

	UFUNCTION(Server, Reliable)
	void Server_SetDead(bool IsDead);

	UFUNCTION(Server, Reliable)
	void Server_SetInputXY(float XInput, float YInput);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_ApplyDeath();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PlayAttackAnim();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PlayHitByAttackAnim(UAnimMontage* MontageToPlay, AActor* AttackedActor);

public:
	bool GetCanFight();
	bool GetIsAiming();
	FVector2D GetInputXY();
	FORCEINLINE bool GetIsDead() const { return bIsDead; }
};
