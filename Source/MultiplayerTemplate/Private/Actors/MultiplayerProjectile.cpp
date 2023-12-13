// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/MultiplayerProjectile.h"
#include "Components/BoxComponent.h"

AMultiplayerProjectile::AMultiplayerProjectile()
{
	PrimaryActorTick.bCanEverTick = true;
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision Box"));
	SetRootComponent(CollisionBox);
	CollisionBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	CollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	CollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
}

void AMultiplayerProjectile::BeginPlay()
{
	Super::BeginPlay();
}

void AMultiplayerProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

