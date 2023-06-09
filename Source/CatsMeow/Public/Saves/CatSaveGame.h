// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "CatSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class CATSMEOW_API UCatSaveGame : public USaveGame
{
	GENERATED_BODY()
public:
	UCatSaveGame() {}
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerData", meta=(AllowPrivateAccess = "true"))
	int32 CatBodyShapeNum = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerData", meta=(AllowPrivateAccess = "true"))
	int32 CatTextureNum = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerData", meta=(AllowPrivateAccess = "true"))
    int32 CatAccessoryNum = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerData", meta=(AllowPrivateAccess = "true"))
	int32 HappinessCurrent = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerData", meta=(AllowPrivateAccess = "true"))
	int32 HungerCurrent = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerData", meta=(AllowPrivateAccess = "true"))
	int32 HygieneCurrent = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerData", meta=(AllowPrivateAccess = "true"))
	int32 MapNumber = 0;
};
