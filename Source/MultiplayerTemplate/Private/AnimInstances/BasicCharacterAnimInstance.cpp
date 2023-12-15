// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstances/BasicCharacterAnimInstance.h"

#include "Characters/BasicCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UBasicCharacterAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
    Super::NativeUpdateAnimation(DeltaTime);

    BasicCharacter = IsValid(BasicCharacter) ? BasicCharacter : Cast<ABasicCharacter>(TryGetPawnOwner());
    if(!IsValid(BasicCharacter)) return;

    float LastFrameSpeed = Speed;
    FVector Velocity = BasicCharacter->GetVelocity();
    Velocity.Z = 0.f;
    Speed = FMath::FInterpTo(LastFrameSpeed, Velocity.Size(), DeltaTime, 6.f);

    ForwardDirectionScalar = BasicCharacter->GetInputXY().Y * Speed;

    RightDirectionScalar = BasicCharacter->GetInputXY().X * Speed;

    bIsInAir = BasicCharacter->GetCharacterMovement()->IsFalling();

    bIsMoving = BasicCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f;

    bCanFight = BasicCharacter->GetCanFight();

    bIsCrouched = BasicCharacter->bIsCrouched;

    bIsAiming = BasicCharacter->GetIsAiming();

    bIsPlayerDead = BasicCharacter->GetIsDead();
}
