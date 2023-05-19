// Fill out your copyright notice in the Description page of Project Settings.


#include "Environment/FoodStall.h"

#include "PaperFlipbookComponent.h"
#include "PaperFlipbook.h"

AFoodStall::AFoodStall()
{
	InitializeFlipbooks();
}

void AFoodStall::BeginPlay()
{
	Super::BeginPlay();

	LoadStallFlipbooks();

	SetFlipbooks();
}

void AFoodStall::MainCharacterInteractFunction(AMainCatCharacter* MainCatRef)
{
	Super::MainCharacterInteractFunction(MainCatRef);

	if (MainCatRef)
	{
		UE_LOG(LogTemp, Warning, TEXT("Stall Mini Game Open!!"));
	}
}

void AFoodStall::LoadStallFlipbooks()
{
	for (int j = 0; j < NumStalls; j++)
	{
		const FString TierStyleValue = FString::FromInt(j);
		const FString AnimationText =
			FString("'/Game/Assets/Outside/Flipbooks/FB_Stall_") +
				TierStyleValue + FString(".FB_Stall_") + TierStyleValue + FString("'");

		UE_LOG(LogTemp, Warning, TEXT("%s"), *AnimationText);
		StallFlipbooks[j] = LoadObject<UPaperFlipbook>(
			nullptr,
			*(AnimationText),
			nullptr,
			LOAD_None,
			nullptr
		);
	}
}

void AFoodStall::InitializeFlipbooks()
{
	StallFlipbooks.Empty();
	for (int j = 0; j < NumStalls; j++)
	{
		StallFlipbooks.Push(TObjectPtr<UPaperFlipbook>());
	}
}

void AFoodStall::SetFlipbooks()
{
	const int32 StyleNum = FMath::RandRange(0, NumStalls-1);
	GetSprite()->SetFlipbook(StallFlipbooks[StyleNum]);
}
