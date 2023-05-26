// Fill out your copyright notice in the Description page of Project Settings.


#include "Environment/Rug.h"

#include "Kismet/GameplayStatics.h"
#include "Saves/CatSaveGame.h"
#include "UI/MiniGameWidget.h"

ARug::ARug()
{
	// HoverWidget = CreateDefaultSubobject<UUserWidget>(TEXT("HoverWidget"));
}

void ARug::MainCharacterInteractFunction(AMainCatCharacter* MainCatCharacter)
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

void ARug::SetCanInteract(bool bInteract)
{
	if (bIsInteractable)
	{
		bCanInteract = bInteract;
	}
}

void ARug::InitializeWidgetAndAddToViewport()
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

		if (CatSaveGame)
		{
			Body = CatSaveGame->CatBodyShapeNum;
			Texture = CatSaveGame->CatTextureNum;
		} else
		{
			Body = 0;
			Texture = 0;
		}
		if (IsValid(MiniGameWidget))
		{
			MiniGameWidget->CatSelection = Body * 2 + Texture;
			MiniGameWidget->AddToViewport();
		}
	}
}

void ARug::BeginPlay()
{
	Super::BeginPlay();
}
