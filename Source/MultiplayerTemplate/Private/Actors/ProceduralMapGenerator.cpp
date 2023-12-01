// Fill out your copyright notice in the Description page of Project Settings.

#include "Actors/ProceduralMapGenerator.h"
#include "ProceduralMeshComponent.h"
#include "Materials/Material.h"
#include "DrawDebugHelpers.h"
#include "KismetProceduralMeshLibrary.h"
#include "Algorithmns/FastNoiseLite.h"

AProceduralMapGenerator::AProceduralMapGenerator()
{
	PrimaryActorTick.bCanEverTick = true;

	ProceduralMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("Proc Generated Mesh"));
	ProceduralMesh->SetupAttachment(GetRootComponent());
	ProceduralMesh->bUseAsyncCooking = true;
	ProceduralMesh->SetCastShadow(false);

	Continentalness = new FastNoiseLite();
	Erosion = new FastNoiseLite();
	Peaks = new FastNoiseLite();
	Temperature = new FastNoiseLite();
	Moisture = new FastNoiseLite();
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

	SetupNoise(Continentalness, 1133, .00006f, 9);
	Continentalness->SetFractalType(FastNoiseLite::FractalType_FBm);
	SetupNoise(Erosion, 2133, .00003f, 9);
	Erosion->SetFractalType(FastNoiseLite::FractalType_FBm);
	SetupNoise(Peaks, 11133, .000009f, 4);
	Peaks->SetFractalType(FastNoiseLite::FractalType_Ridged);
	SetupNoise(Temperature, 12337, .000001f, 3);
	Temperature->SetFractalType(FastNoiseLite::FractalType_FBm);
	SetupNoise(Moisture, 18337, .0008f, 3);
	Moisture->SetFractalType(FastNoiseLite::FractalType_FBm);

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
	ProceduralMesh->CreateMeshSection_LinearColor(0, Vertices, Triangles, Normals, UVs, VertexColor, Tangents, true);
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
			float XPosition = (X * Scale) - ((XSize / 2) * Scale) + GetActorLocation().X;
			float YPosition = (Y * Scale) - ((YSize / 2) * Scale) + GetActorLocation().Y;
			float VertexHeight = CalculateHeight(XPosition, YPosition);
			//FMath::PerlinNoise2D(FVector2D(X * NoiseScale + .1, Y * NoiseScale + .1)) * ZScale)
			if(VertexHeight < 1000)
			{
				VertexColor.Add(FLinearColor(FColor(0, 0, 1)));
			}
			else
			{
				VertexColor.Add(FLinearColor(FColor(0, 1, 0)));
			}

			Vertices.Add(FVector(XPosition, YPosition, VertexHeight));
			UVs.Add(FVector2D((X * UVScale)  - ((XSize / 2) * UVScale ), (Y * UVScale) - ((YSize / 2) * UVScale)));

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

void AProceduralMapGenerator::SetupNoise(FastNoiseLite *Noise, int Seed, float Frequency, int Octaves)
{
	Noise->SetSeed(Seed);
	Noise->SetFrequency(Frequency);
	Noise->SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2S);
	Noise->SetFractalOctaves(Octaves);

}
float AProceduralMapGenerator::CalculateHeight(float XCoordinate, float YCoordinate)
{
	float ContinentalFactor = Continentalness->GetNoise(XCoordinate, YCoordinate) * ZScale;
	float ErosianFactor = Erosion->GetNoise(XCoordinate, YCoordinate) * ZScale;
	float PeaksFactor = Peaks->GetNoise(XCoordinate, YCoordinate) * ZScale;

	float result = 
	ContinentalCurve->GetFloatValue(ContinentalFactor) * 1.1f +
	ErosionCurve->GetFloatValue(ErosianFactor) * 1.6f +
	PeaksCurve->GetFloatValue(PeaksFactor) * 1.3f;
				
    return result;
}
