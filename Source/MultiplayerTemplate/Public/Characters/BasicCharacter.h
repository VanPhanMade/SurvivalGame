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

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

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
	
	bool bInGameMenuOpen = false;

	/** Input Callback Functions */
	void StartSession();
	void OpenInGameMenu();
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Interact();

	// Replicated Variables
	UPROPERTY()
	class AInteractable* LastInteractable;

	// RPCs
	UFUNCTION(Server, Reliable)
	void Server_DestroyActor(AActor* ActorToDestroy);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_DestroyActor(AActor* ActorToDestroy);
};
