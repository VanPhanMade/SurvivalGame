// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/ReplicatingCube.h"
#include "Components/SphereComponent.h"
#include "Net/UnrealNetwork.h"

AReplicatingCube::AReplicatingCube()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Collision Sphere"));
	CollisionSphere->SetGenerateOverlapEvents(true);
	CollisionSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	CollisionSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetRootComponent(CollisionSphere);

	// For replicating objects we disable physics and only enable physics / collisions on the server-end
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	Mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Mesh->SetupAttachment(RootComponent);
	
}

void AReplicatingCube::BeginPlay()
{
	if(HasAuthority())
	{
		CollisionSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		CollisionSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
		CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::AssignMeshRandomColor);
		// AreaSphere->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnSphereEndOverlap);
		// ShowPickupWidget(false);
	}
	
}

void AReplicatingCube::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AReplicatingCube::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(AReplicatingCube, MeshColor, COND_None);
}

void AReplicatingCube::AssignMeshRandomColor(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult) 
{
	MeshColor = FColor::MakeRandomColor();
	UMaterialInstanceDynamic* NewMeshMaterial = UMaterialInstanceDynamic::Create(Mesh->GetMaterial(0), this);
	NewMeshMaterial->SetVectorParameterValue(TEXT("Tint"), MeshColor);
	Mesh->SetMaterial(0, NewMeshMaterial);
}

void AReplicatingCube::OnRep_MeshColor()
{
	UMaterialInstanceDynamic* NewMeshMaterial = UMaterialInstanceDynamic::Create(Mesh->GetMaterial(0), this);
	NewMeshMaterial->SetVectorParameterValue(TEXT("Tint"), MeshColor);
	Mesh->SetMaterial(0, NewMeshMaterial);
}
