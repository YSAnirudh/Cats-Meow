// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Environment/BaseEnvironmentActor.h"
#include "FoodStall.generated.h"

/**
 * 
 */
UCLASS()
class CATSMEOW_API AFoodStall : public ABaseEnvironmentActor
{
	GENERATED_BODY()
public:
	// FUNCTIONS
	AFoodStall();
	
	// VARIABLES
	protected:
	// FUNCTIONS
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void MainCharacterInteractFunction(AMainCatCharacter* MainCatCharacter) override;

	UFUNCTION()
	void InitializeWidgetAndAddToViewport();
	
	virtual void SetCanInteract(bool bInteract) override;
	// VARIABLES
	// Flipbooks for Storing Animations of 2D Sprites
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CatCharacter|Config")
	TArray<TObjectPtr<class UPaperFlipbook>> StallFlipbooks;
private:
	// FUNCTIONS
	void LoadStallFlipbooks();

	void InitializeFlipbooks();

	void SetFlipbooks();

	// VARIABLES
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BuildingStyle", meta = (AllowPrivateAccess = "true"))
	int32 NumStalls = 2;
};
