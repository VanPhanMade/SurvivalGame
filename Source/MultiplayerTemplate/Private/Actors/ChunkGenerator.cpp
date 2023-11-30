// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/ChunkGenerator.h"
#include "Actors/ProceduralMapGenerator.h"

AChunkGenerator::AChunkGenerator()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AChunkGenerator::BeginPlay()
{
	Super::BeginPlay();
	FTransform SecondTransform = GetActorTransform();
	SecondTransform.SetTranslation(FVector(5000, 0,0));
	AProceduralMapGenerator* Chunk = GetWorld()->SpawnActor<AProceduralMapGenerator>(ChunkWidget, GetActorTransform());
	AProceduralMapGenerator* Chunk2 = GetWorld()->SpawnActor<AProceduralMapGenerator>(ChunkWidget, SecondTransform);
	Chunk->CreateMapChunk();
	Chunk2->CreateMapChunk();
}

void AChunkGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

