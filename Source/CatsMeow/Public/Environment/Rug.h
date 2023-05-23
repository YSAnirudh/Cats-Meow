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
	virtual void MainCharacterInteractFunction(AMainCatCharacter* MainCatCharacter) override;

	virtual void SetCanInteract(bool bInteract) override;
	// VARIABLES
	protected:
	// FUNCTIONS

	virtual void BeginPlay() override;

	// VARIABLES
	private:
	// FUNCTIONS

	// VARIABLES
};
