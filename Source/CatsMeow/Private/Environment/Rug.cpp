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
	if (bIsInteractable && !bHasPlayedMiniGame)
	{
		UE_LOG(LogTemp, Warning, TEXT("Rug Mini Game Open!!"));
		if (!IsValid(MiniGameWidget))
		{
			UE_LOG(LogTemp, Warning, TEXT("No Widget"));
			return;
		}
		if (IsValid(MiniGameWidget) && !MiniGameWidget->IsInViewport())
		{
			MiniGameWidget->AddToViewport();
			bHasPlayedMiniGame = true;
			bCanInteract = false;
			if (MainCatCharacter)
			{
				MainCatCharacter->DisableInput(MainCatCharacter->GetLocalViewingPlayerController());
				MainCatCharacter->RemoveInteractableFromSet(this);
			}
		}
	} else
	{
		UE_LOG(LogTemp, Warning, TEXT("Already Played!!"));
	}
}

void ARug::SetCanInteract(bool bInteract)
{
	if (bIsInteractable)
	{
		bCanInteract = bInteract;
	}
}

void ARug::BeginPlay()
{
	Super::BeginPlay();

	if (!MiniGameWidget && MiniGameWidgetClass)
	{
		MiniGameWidget = CreateWidget<UMiniGameWidget>(GetWorld(), MiniGameWidgetClass, TEXT("PettingGameWidget"));
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

		if (CatSaveGame)
		{
			int32 Body = CatSaveGame->CatBodyShapeNum;
			int32 Texture = CatSaveGame->CatTextureNum;

			if (IsValid(MiniGameWidget))
			{
				MiniGameWidget->CatSelection = Body * 2 + Texture;
			}
		}
	}
}
