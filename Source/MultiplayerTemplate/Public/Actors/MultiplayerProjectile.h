// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MultiplayerProjectile.generated.h"

UCLASS()
class MULTIPLAYERTEMPLATE_API AMultiplayerProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	AMultiplayerProjectile();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	

private:
	UPROPERTY(EditAnywhere)
	class UBoxComponent* CollisionBox;
};
