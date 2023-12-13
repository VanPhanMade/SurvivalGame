// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ReplicatingCube.generated.h"

UCLASS()
class MULTIPLAYERTEMPLATE_API AReplicatingCube : public AActor
{
	GENERATED_BODY()
	
public:	
	AReplicatingCube();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess="true"))
	UStaticMeshComponent* Mesh;
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess="true"))
	class USphereComponent* CollisionSphere;

	UPROPERTY(ReplicatedUsing = OnRep_MeshColor)
	FColor MeshColor;

	UFUNCTION()
	void OnRep_MeshColor();

	UFUNCTION()
	void AssignMeshRandomColor(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
