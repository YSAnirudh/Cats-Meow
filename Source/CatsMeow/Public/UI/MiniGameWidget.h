// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MiniGameWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGameOverDelegate, bool, bCatWon);

/**
 * 
 */
UCLASS()
class CATSMEOW_API UMiniGameWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	// FUNCTIONS
	// VARIABLES
	UPROPERTY(BlueprintCallable)
	FGameOverDelegate GameOverDelegate;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Result")
	bool bWon = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Result")
	int32 CatSelection = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Result")
	int32 AICatSelection = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Result")
	int32 AICatAccessory = 0;
protected:
	// FUNCTIONS
	// VARIABLES
private:
	// FUNCTIONS
	// VARIABLES
};
