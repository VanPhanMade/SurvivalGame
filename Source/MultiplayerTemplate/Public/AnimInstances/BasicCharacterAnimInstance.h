// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BasicCharacterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERTEMPLATE_API UBasicCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:


protected:
//virtual void NativeInitializeAnimation() override;
virtual void NativeUpdateAnimation(float DeltaTime) override;

private:
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	class ABasicCharacter* BasicCharacter;
	
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	float Speed;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	float ForwardDirectionScalar;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	float RightDirectionScalar;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	bool bIsInAir;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	bool bIsMoving;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	bool bCanFight;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	bool bIsCrouched;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	bool bIsAiming;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	bool bIsPlayerDead;
};
