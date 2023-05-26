// Fill out your copyright notice in the Description page of Project Settings.


#include "Environment/FoodStall.h"

#include "PaperFlipbookComponent.h"
#include "PaperFlipbook.h"
#include "Kismet/GameplayStatics.h"
#include "Saves/CatSaveGame.h"
#include "UI/MiniGameWidget.h"

AFoodStall::AFoodStall()
{
	InitializeFlipbooks();

	bIsInteractable = true;
}

void AFoodStall::BeginPlay()
{
	Super::BeginPlay();

	LoadStallFlipbooks();

	SetFlipbooks();
}

void AFoodStall::MainCharacterInteractFunction(AMainCatCharacter* MainCatCharacter)
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

void AFoodStall::InitializeWidgetAndAddToViewport()
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

void AFoodStall::SetCanInteract(bool bInteract)
{
	if (bIsInteractable)
	{
		bCanInteract = bInteract;
	}
}

void AFoodStall::LoadStallFlipbooks()
{
	const FString TierStyleValue = FString::FromInt(1);
	const FString AnimationText =
		FString("'/Game/Assets/Outside/Flipbooks/FB_Stall_") +
			TierStyleValue + FString(".FB_Stall_") + TierStyleValue + FString("'");

	StallFlipbooks[0] = LoadObject<UPaperFlipbook>(
		nullptr,
		*(AnimationText),
		nullptr,
		LOAD_None,
		nullptr
	);
}

void AFoodStall::InitializeFlipbooks()
{
	StallFlipbooks.Empty();
	StallFlipbooks.Push(TObjectPtr<UPaperFlipbook>());
}

void AFoodStall::SetFlipbooks()
{
	GetSprite()->SetFlipbook(StallFlipbooks[0]);
}
