// Fill out your copyright notice in the Description page of Project Settings.


#include "Environment/Rug.h"

#include "Kismet/GameplayStatics.h"
#include "Saves/CatSaveGame.h"
#include "UI/MiniGameWidget.h"

ARug::ARug()
{
	// HoverWidget = CreateDefaultSubobject<UUserWidget>(TEXT("HoverWidget"));
}

void ARug::MainCharacterInteractFunction()
{
	if (bIsInteractable && !bHasPlayedMiniGame)
	{
		UE_LOG(LogTemp, Warning, TEXT("Rug Mini Game Open!!"));
		if (MiniGameWidget && !MiniGameWidget->IsInViewport())
		{
			MiniGameWidget->AddToViewport();
			bHasPlayedMiniGame = true;
			AMainCatCharacter* MainCatRef = Cast<AMainCatCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
			if (MainCatRef)
			{
				bCanInteract = false;
				MainCatRef->DisableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));
				MainCatRef->RemoveInteractableFromSet(this);
			}
		}
	} else
	{
		UE_LOG(LogTemp, Warning, TEXT("Already Played!!"));
	}
}

void ARug::BeginPlay()
{
	Super::BeginPlay();

	if (!MiniGameWidget && MiniGameWidgetClass)
	{
		MiniGameWidget = CreateWidget<UMiniGameWidget>(GetWorld(), MiniGameWidgetClass, TEXT("PettingGameWidget"));
		UCatSaveGame* CatSaveGame = Cast<UCatSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("MainSlot"), 0));
		int32 Body = CatSaveGame->CatBodyShapeNum;
		int32 Texture = CatSaveGame->CatTextureNum;
		
		MiniGameWidget->CatSelection = Body * 2 + Texture;
	}
}

void ARug::OnMiniGameFinish(bool bCatWon)
{
	bHasPlayedMiniGame = true;
	AMainCatCharacter* MainCatRef = Cast<AMainCatCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if (MainCatRef)
	{
		MainCatRef->EnableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		if (bCatWon)
		{
			MainCatRef->IncrementHygiene();
		}
	}
	if (MiniGameWidget)
	{
		MiniGameWidget->RemoveFromViewport();
	}
}
