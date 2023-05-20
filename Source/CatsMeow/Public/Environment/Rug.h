// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Environment/BaseEnvironmentActor.h"
#include "Rug.generated.h"

/**
 * 
 */
UCLASS()
class CATSMEOW_API ARug : public ABaseEnvironmentActor
{
	GENERATED_BODY()
public:
	// FUNCTIONS
	ARug();
	virtual void MainCharacterInteractFunction() override;
	
	// VARIABLES
	protected:
	// FUNCTIONS

	virtual void BeginPlay() override;

	// VARIABLES
	private:
	// FUNCTIONS
	UFUNCTION()
	void OnMiniGameFinish(bool bCatWon);

	// VARIABLES
};
