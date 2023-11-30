// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "ProceduralMapGenerator.generated.h"

UCLASS()
class MULTIPLAYERTEMPLATE_API AProceduralMapGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	AProceduralMapGenerator();

	void CreateMapChunk();
	FVector2D CoordinatePosition();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void OnConstruction(const FTransform& Transform) override;
	

	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess="true"))
	class UMaterial* MeshMaterial;

	UPROPERTY(EditAnywhere, meta=(ClampMin = 0))
	int32 XSize = 50;
	UPROPERTY(EditAnywhere, meta=(ClampMin = 0))
	int32 YSize = 50;
	UPROPERTY(EditAnywhere, meta=(ClampMin = 0))
	float ZScale = 400.f;
	UPROPERTY(EditAnywhere, meta=(ClampMin = 0))
	float NoiseScale = .1f; // Lower scale results in larger flat pools

	UPROPERTY(EditAnywhere, meta=(ClampMin = 0.0001))
	float Scale = 200;	// Distance between each coordinate point
	UPROPERTY(EditAnywhere, meta=(ClampMin = 0.0001))
	float UVScale = 1;	// Stretch of UV maps, bigger = smaller tesselation of materials
	
private:
	class UProceduralMeshComponent * ProceduralMesh;
	TArray<FVector> Vertices;
	TArray<int> Triangles;
	TArray<FVector2D> UVs;
	TArray<FVector> Normals;
	TArray<struct FProcMeshTangent> Tangents;
	
	void CreateMesh();
	void CreateVertices();
	void CreateTriangles();
	

	int32 MeshSectionIndex = 0;
};
