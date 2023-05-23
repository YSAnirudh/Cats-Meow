// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Environment/BaseEnvironmentActor.h"
#include "TwoSpriteEnvironment.generated.h"

/**
 * 
 */
UCLASS()
class CATSMEOW_API ATwoSpriteEnvironment : public ABaseEnvironmentActor
{
	GENERATED_BODY()
public:
	// FUNCTIONS
	ATwoSpriteEnvironment();
	
	// VARIABLES
protected:
	// FUNCTIONS
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// VARIABLES
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CatCharacter|Config")
	TArray<TObjectPtr<class UPaperFlipbook>> AssetFlipbooks;
private:
	// FUNCTIONS
	void LoadFlipbooks();

	void InitializeFlipbooks();

	void SetFlipbooks();
	
	// VARIABLES
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BuildingStyle", meta = (AllowPrivateAccess = "true"))
	int32 NumAssets = 2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BuildingStyle", meta = (AllowPrivateAccess = "true"))
	FString AssetPrefix = TEXT("");
};
