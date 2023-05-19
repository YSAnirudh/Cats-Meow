// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Environment/BaseEnvironmentActor.h"
#include "ApartmentDoor.generated.h"

/**
 * 
 */
UCLASS()
class CATSMEOW_API AApartmentDoor : public ABaseEnvironmentActor
{
	GENERATED_BODY()
public:
	// FUNCTIONS
	AApartmentDoor();
	
	// VARIABLES
protected:
	// FUNCTIONS
	virtual void MainCharacterInteractFunction(class AMainCatCharacter* MainCatRef) override;

	// VARIABLES
private:
	// FUNCTIONS

	// VARIABLES
};
