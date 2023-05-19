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
	
	// VARIABLES
	protected:
	// FUNCTIONS
	virtual void MainCharacterInteractFunction() override;

	virtual void BeginPlay() override;

	// VARIABLES
	private:
	// FUNCTIONS
	UFUNCTION()
	void OnMiniGameFinish(bool bCatWon);

	// VARIABLES
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction", meta = (AllowPrivateAccess = "true"))
	bool bHasPlayedMiniGame = false;

	class UMiniGameWidget* MiniGameWidget = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MiniGame", meta = (AllowPrivateAccess = "true"))
	class UUserWidget* HoverWidget = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MiniGame", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UMiniGameWidget> MiniGameWidgetClass;
};
