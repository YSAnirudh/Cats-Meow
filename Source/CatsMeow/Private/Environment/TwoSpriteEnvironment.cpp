// Fill out your copyright notice in the Description page of Project Settings.


#include "Environment/TwoSpriteEnvironment.h"

#include "PaperFlipbookComponent.h"

ATwoSpriteEnvironment::ATwoSpriteEnvironment()
{
	

	bIsInteractable = false;
}

void ATwoSpriteEnvironment::BeginPlay()
{
	Super::BeginPlay();

	InitializeFlipbooks();
	
	LoadFlipbooks();

	SetFlipbooks();
}

void ATwoSpriteEnvironment::LoadFlipbooks()
{
	for (int i = 0; i < NumAssets; i++)
	{
		const FString TierStyleValue = FString::FromInt(i);
		const FString AnimationText =
			FString("'/Game/Assets/Outside/Flipbooks/") + AssetPrefix + FString("_") +
				TierStyleValue + FString(".") + AssetPrefix + FString("_") + TierStyleValue + FString("'");

		AssetFlipbooks[i] = LoadObject<UPaperFlipbook>(
			nullptr,
			*(AnimationText),
			nullptr,
			LOAD_None,
			nullptr
		);
	}
}

void ATwoSpriteEnvironment::InitializeFlipbooks()
{
	AssetFlipbooks.Empty();
	for (int i = 0; i < NumAssets; i++)
	{
		AssetFlipbooks.Push(TObjectPtr<UPaperFlipbook>());
	}
}

void ATwoSpriteEnvironment::SetFlipbooks()
{
	const int32 StyleNum = FMath::RandRange(0, NumAssets-1);
	GetSprite()->SetFlipbook(AssetFlipbooks[StyleNum]);
}
