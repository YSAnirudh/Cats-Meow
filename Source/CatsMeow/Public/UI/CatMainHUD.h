// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Blueprint/UserWidget.h"
#include "CatMainHUD.generated.h"


/**
 * 
 */
UCLASS()
class CATSMEOW_API ACatMainHUD : public AHUD
{
	GENERATED_BODY()
public:
	// FUNCTIONS
	ACatMainHUD();

	virtual void DrawHUD() override;

	UFUNCTION(BlueprintCallable)
	void ShowStatBar();
	UFUNCTION(BlueprintCallable)
	void HideStatBar();

	UFUNCTION(BlueprintCallable)
	void ShowCatCustomizationScreen();
	UFUNCTION(BlueprintCallable)
	void HideCatCustomizationScreen();
	
	// VARIABLES
protected:
	// FUNCTIONS
	// Called when the game starts
	virtual void BeginPlay() override;

	// VARIABLES
	// Initial Customization Screen
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="StatBars", meta=(AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> CatCustomizationMenuWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="StatBars", meta=(AllowPrivateAccess = "true"))
	UUserWidget* CatCustomizationMenuWidgetInstance = nullptr;
	
	// Stat Bars
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="StatBars", meta=(AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> StatBarWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="StatBars", meta=(AllowPrivateAccess = "true"))
	UUserWidget* StatBarWidgetInstance = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="StatBars", meta=(AllowPrivateAccess = "true"))
	TArray<UUserWidget*> WidgetList;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="StatBars", meta=(AllowPrivateAccess = "true"))
	UUserWidget* ActiveWidget;
private:
	// FUNCTIONS

	// VARIABLES
};
