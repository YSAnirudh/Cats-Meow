// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Environment/BaseEnvironmentActor.h"
#include "BuildingEnvironment.generated.h"

/**
 * 
 */
UCLASS()
class CATSMEOW_API ABuildingEnvironment : public ABaseEnvironmentActor
{
	GENERATED_BODY()
public:
	// FUNCTIONS
	ABuildingEnvironment();
	
	// VARIABLES
protected:
	// FUNCTIONS
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void MainCharacterInteractFunction() override;
	// VARIABLES
	// Flipbooks for Storing Animations of 2D Sprites
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CatCharacter|Config")
	TArray<TObjectPtr<class UPaperFlipbook>> BuildingFlipbooks;
private:
	// FUNCTIONS
	void LoadBuildingFlipbooks();

	void InitializeFlipbooks();

	void SetFlipbooks();

	// VARIABLES
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Environment", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UPaperFlipbookComponent> Tier1Sprite;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Environment", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UPaperFlipbookComponent> Tier2Sprite;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BuildingStyle", meta = (AllowPrivateAccess = "true"))
	int32 NumBuildingTiers = 3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BuildingStyle", meta = (AllowPrivateAccess = "true"))
	int32 NumBuildingAssets = 3;
};
