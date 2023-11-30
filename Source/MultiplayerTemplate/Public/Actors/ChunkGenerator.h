// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ChunkGenerator.generated.h"

UCLASS()
class MULTIPLAYERTEMPLATE_API AChunkGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	AChunkGenerator();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AProceduralMapGenerator> ChunkWidget;

};
