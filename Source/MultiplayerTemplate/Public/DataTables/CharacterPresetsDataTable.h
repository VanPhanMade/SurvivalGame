// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "CharacterPresetsDataTable.generated.h"

USTRUCT(BlueprintType, Category=Stats)
struct FCharacterPresets : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	public:

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName PresetName;

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USkeletalMesh* CharacterModel;

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UTexture2D* CharacterImage;

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FText CharacterDescription;

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USoundBase* SelectionAudio;
};
/**
 * 
 */
UCLASS()
class MULTIPLAYERTEMPLATE_API UCharacterPresetsDataTable : public UDataTable
{
	GENERATED_BODY()
	
public:

protected:

private:

};
