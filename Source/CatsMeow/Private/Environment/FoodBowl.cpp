// Fill out your copyright notice in the Description page of Project Settings.


#include "Environment/FoodBowl.h"

#include "Kismet/GameplayStatics.h"
#include "Saves/CatSaveGame.h"
#include "UI/MiniGameWidget.h"

AFoodBowl::AFoodBowl()
{
	bIsInteractable = true;
}

void AFoodBowl::MainCharacterInteractFunction(AMainCatCharacter* MainCatCharacter)
{
	if (bIsInteractable && !bHasPlayedMiniGame && MiniGameWidgetClass)
	{
		bHasPlayedMiniGame = true;
		bCanInteract = false;
		if (MainCatCharacter)
		{
			MainCatCharacter->DisableInput(MainCatCharacter->GetLocalViewingPlayerController());
			MainCatCharacter->RemoveInteractableFromSet(this);
		}
		InitializeWidgetAndAddToViewport();
	}
}

void AFoodBowl::InitializeWidgetAndAddToViewport()
{
	if (!MiniGameWidget && MiniGameWidgetClass)
	{
		MiniGameWidget = CreateWidget<UMiniGameWidget>(GetWorld(), MiniGameWidgetClass);
		if (!MiniGameWidget) {
		    return;
		}
		UCatSaveGame* CatSaveGame = Cast<UCatSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("MainSlot"), 0));
		int32 Body;
		int32 Texture;
		int32 Accessory;

		if (CatSaveGame)
		{
			Body = CatSaveGame->CatBodyShapeNum;
			Texture = CatSaveGame->CatTextureNum;
			Accessory = CatSaveGame->CatAccessoryNum;
		} else
		{
			Body = 0;
			Texture = 0;
			Accessory = 0;
		}
		if (IsValid(MiniGameWidget))
		{
			MiniGameWidget->CatSelection = Body * 2 + Texture;
			MiniGameWidget->CatAccessory = Accessory;
			MiniGameWidget->AddToViewport();
		}
	}
}

void AFoodBowl::SetCanInteract(bool bInteract)
{
	if (bIsInteractable)
	{
		bCanInteract = bInteract;
	}
}
