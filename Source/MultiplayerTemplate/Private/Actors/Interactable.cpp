// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Interactable.h"
#include "Net/UnrealNetwork.h"

#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Character.h"

AInteractable::AInteractable()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Interaction Collision Radius"));
	CollisionSphere->SetGenerateOverlapEvents(true);
	CollisionSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	CollisionSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetRootComponent(CollisionSphere);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Mesh->SetupAttachment(RootComponent);

	PickupWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget Component"));
	PickupWidget->SetupAttachment(RootComponent);
}

void AInteractable::BeginPlay()
{
	Super::BeginPlay();

	CollisionSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnSphereOverlap);
	CollisionSphere->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnSphereEndOverlap);
	OnDestroyed.AddDynamic(this, &ThisClass::OnInteractableDestroyed);
	PickupWidget->SetVisibility(false);
}

void AInteractable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AInteractable::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

}
void AInteractable::OnSphereOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if(Cast<ACharacter>(OtherActor)->IsLocallyControlled())
	{
		if(PickupWidget) PickupWidget->SetVisibility(true);
	}
}

void AInteractable::OnSphereEndOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex)
{
	if(Cast<ACharacter>(OtherActor)->IsLocallyControlled())
	{
		if(PickupWidget) PickupWidget->SetVisibility(false);
	}
}

void AInteractable::OnInteractableDestroyed(AActor *This)
{
	CollisionSphere->OnComponentBeginOverlap.RemoveDynamic(this, &ThisClass::OnSphereOverlap);
	CollisionSphere->OnComponentEndOverlap.RemoveDynamic(this, &ThisClass::OnSphereEndOverlap);
	OnDestroyed.RemoveDynamic(this, &ThisClass::OnInteractableDestroyed);
}
