// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Environment/BaseEnvironmentActor.h"
#include "FoodBowl.generated.h"

/**
 * 
 */
UCLASS()
class CATSMEOW_API AFoodBowl : public ABaseEnvironmentActor
{
	GENERATED_BODY()
public:
	// FUNCTIONS
	AFoodBowl();
	
	// VARIABLES
protected:
	// FUNCTIONS
	virtual void MainCharacterInteractFunction(class AMainCatCharacter* MainCatRef) override;

	// VARIABLES
private:
	// FUNCTIONS

	// VARIABLES
};
