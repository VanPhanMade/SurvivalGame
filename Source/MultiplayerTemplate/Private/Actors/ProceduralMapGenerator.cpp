// Fill out your copyright notice in the Description page of Project Settings.

#include "Actors/ProceduralMapGenerator.h"
#include "ProceduralMeshComponent.h"
#include "Materials/Material.h"
#include "DrawDebugHelpers.h"
#include "KismetProceduralMeshLibrary.h"

AProceduralMapGenerator::AProceduralMapGenerator()
{
	PrimaryActorTick.bCanEverTick = true;

	ProceduralMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("Proc Generated Mesh"));
	ProceduralMesh->SetupAttachment(GetRootComponent());
}

void AProceduralMapGenerator::CreateMapChunk()
{
	
	Vertices.Reset();
	Triangles.Reset();
	UVs.Reset();
	Normals.Reset();
	Tangents.Reset();

	CreateVertices();
	CreateTriangles();
	UKismetProceduralMeshLibrary::CalculateTangentsForMesh(Vertices, Triangles, UVs, Normals, Tangents);
	CreateMesh();
	CoordinatePosition();
}

FVector2D AProceduralMapGenerator::CoordinatePosition()
{
	int32 AspectRatio = (XSize + YSize) / 2;
	float ActorXPosition = GetActorLocation().X;
	float ActorYPosition = GetActorLocation().Y;
	FVector2D Coordinates(ActorXPosition / AspectRatio, ActorYPosition / AspectRatio);

	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Blue, FString::Printf(TEXT("Map chunk coords: %f , %f"), Coordinates.X, Coordinates.Y));
    return Coordinates;
}
void AProceduralMapGenerator::BeginPlay()
{
	Super::BeginPlay();
	CreateMapChunk();
}

void AProceduralMapGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AProceduralMapGenerator::OnConstruction(const FTransform &Transform)
{
	Super::OnConstruction(Transform);
	// Uncomment to view chunks in editor vs inside engine
	/*
	CreateMapChunk();
	*/
	
}

void AProceduralMapGenerator::CreateMesh()
{
	ProceduralMesh->CreateMeshSection_LinearColor(0, Vertices, Triangles, Normals, UVs, TArray<FLinearColor>(), Tangents, true);
	if(MeshMaterial)
	{
		ProceduralMesh->SetMaterial(0, MeshMaterial);
	}
}

void AProceduralMapGenerator::CreateVertices()
{
	for(int32 X = 0; X <= XSize; ++X)
	{
		for(int32 Y = 0; Y <= YSize; ++Y)
		{
			float ActorXPosition = GetTransform().GetTranslation().X;
			float ActorYPosition = GetTransform().GetTranslation().Y;

			//Subtraction in X and Y represent the offset to center each chunk
			Vertices.Add(
				FVector((X * Scale) - ((XSize / 2) * Scale) + ActorXPosition,
				(Y * Scale) - ((YSize / 2) * Scale) + ActorYPosition,
				FMath::PerlinNoise2D(FVector2D(X * NoiseScale + .1, Y * NoiseScale + .1)) * ZScale));
			UVs.Add(
				FVector2D((X * UVScale)  - ((XSize / 2) * UVScale ), (Y * UVScale) - ((YSize / 2) * UVScale)));
		}
	}
}

void AProceduralMapGenerator::CreateTriangles()
{
	int VertexIndex = 0;

	for(int32 X = 0; X < XSize; ++X)
	{
		for(int32 Y = 0; Y < YSize; ++Y)
		{
			Triangles.Add(VertexIndex);
			Triangles.Add(VertexIndex + 1);
			Triangles.Add(VertexIndex + YSize + 1);
			Triangles.Add(VertexIndex + 1);
			Triangles.Add(VertexIndex + YSize + 2);
			Triangles.Add(VertexIndex + YSize + 1);
			++VertexIndex;
		}
		++VertexIndex;
	}

	
	
}
