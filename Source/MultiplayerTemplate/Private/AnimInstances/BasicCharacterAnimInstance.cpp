// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstances/BasicCharacterAnimInstance.h"

#include "Characters/BasicCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

// void UBasicCharacterAnimInstance::NativeInitializeAnimation()
// {
//     Super::NativeIntializeAnimation();

//     BasicCharacter = Cast<ABasicCharacter>(TryGetPawnOwner());
// }
void UBasicCharacterAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
    Super::NativeUpdateAnimation(DeltaTime);

    BasicCharacter = IsValid(BasicCharacter) ? BasicCharacter : Cast<ABasicCharacter>(TryGetPawnOwner());
    if(!IsValid(BasicCharacter)) return;

    FVector Velocity = BasicCharacter->GetVelocity();
    Velocity.Z = 0.f;
    Speed = Velocity.Size();

    bIsInAir = BasicCharacter->GetCharacterMovement()->IsFalling();

    bIsMoving = BasicCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f;

}
