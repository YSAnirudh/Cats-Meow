// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Volume.h"
#include "PCGSystem/PCGComponent.h"
#include "PCGVolume.generated.h"

/**
 * 
 */
UCLASS()
class CATSMEOW_API APCGVolume : public AVolume
{
	GENERATED_BODY()
public:
	// FUNCTIONS
	APCGVolume();

	// VARIABLES
protected:
	// FUNCTIONS

	// VARIABLES
private:
	// FUNCTIONS

	// VARIABLES
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UPCGComponent> PCGComponent = nullptr;

};
