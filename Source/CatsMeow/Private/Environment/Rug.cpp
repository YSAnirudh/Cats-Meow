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
	Super::MainCharacterInteractFunction();

	if (!bHasPlayedMiniGame)
	{
		AMainCatCharacter* MainCatRef = Cast<AMainCatCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
		if (MainCatRef)
		{
			MainCatRef->DisableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		}

		UE_LOG(LogTemp, Warning, TEXT("Rug Mini Game Open!!"));
		if (MiniGameWidget)
		{
			MiniGameWidget->AddToViewport();
		}
	} else
	{
		
		UE_LOG(LogTemp, Warning, TEXT("Already Played!!"));
	}
	// Open Widget for MiniGame
}

void ARug::BeginPlay()
{
	Super::BeginPlay();

	if (!MiniGameWidget && MiniGameWidgetClass)
	{
		MiniGameWidget = CreateWidget<UMiniGameWidget>(GetWorld(), MiniGameWidgetClass, TEXT("PettingGameWidget"));
		MiniGameWidget->GameOverDelegate.AddDynamic(this, &ARug::OnMiniGameFinish);
		UCatSaveGame* CatSaveGame = Cast<UCatSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("MainSlot"), 0));
		int32 Body = CatSaveGame->CatBodyShapeNum;
		int32 Texture = CatSaveGame->CatTextureNum;
		
		MiniGameWidget->CatSelection = Body * 2 + Texture;
	}
}

void ARug::OnMiniGameFinish(bool bCatWon)
{
	bHasPlayedMiniGame = true;
	if (bCatWon)
	{
		AMainCatCharacter* MainCatRef = Cast<AMainCatCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
		if (MainCatRef)
		{
			MainCatRef->EnableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));
			MainCatRef->IncrementHappiness();
		}
	}
	if (MiniGameWidget)
	{
		MiniGameWidget->RemoveFromViewport();
	}
}
