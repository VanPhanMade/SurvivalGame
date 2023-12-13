// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.generated.h"

UCLASS()
class MULTIPLAYERTEMPLATE_API AInteractable : public AActor
{
	GENERATED_BODY()
	
public:	
	AInteractable();
	friend class ABasicCharacter;

protected:
	// AActor
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Event callbacks
	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	virtual void OnInteractableDestroyed(AActor* This);

private:
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess="true"))
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess="true"))
	class USphereComponent* CollisionSphere;

	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess="true"))
	class UWidgetComponent* PickupWidget;


};
