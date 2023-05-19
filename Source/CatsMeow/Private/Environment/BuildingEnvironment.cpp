// Fill out your copyright notice in the Description page of Project Settings.


#include "Environment/BuildingEnvironment.h"

#include "PaperFlipbook.h"
#include "PaperFlipbookComponent.h"

ABuildingEnvironment::ABuildingEnvironment() : ABaseEnvironmentActor()
{
	Tier1Sprite = CreateDefaultSubobject<UPaperFlipbookComponent>("Tier1Sprite");
	Tier1Sprite->SetupAttachment(GetSprite());

	Tier2Sprite = CreateDefaultSubobject<UPaperFlipbookComponent>("Tier2Sprite");
	Tier2Sprite->SetupAttachment(Tier1Sprite);

	InitializeFlipbooks();
}

void ABuildingEnvironment::BeginPlay()
{
	Super::BeginPlay();

	LoadBuildingFlipbooks();

	SetFlipbooks();
}

void ABuildingEnvironment::MainCharacterInteractFunction(AMainCatCharacter* MainCatRef)
{
	Super::MainCharacterInteractFunction(MainCatRef);
}

void ABuildingEnvironment::LoadBuildingFlipbooks()
{
	//PaperFlipbook'/Game/Assets/Outside/Flipbooks/Buildings/FB_Building_0_0.FB_Building_0_0'
	for (int i = 0; i < NumBuildingTiers; i++)
	{
		for (int j = 0; j < NumBuildingAssets; j++)
		{
			const FString TierStyleValue = FString::FromInt(i) + FString("_") + FString::FromInt(j);
			const FString AnimationText =
				FString("'/Game/Assets/Outside/Flipbooks/Buildings/FB_Building_") +
					TierStyleValue + FString(".FB_Building_") + TierStyleValue + FString("'");

			UE_LOG(LogTemp, Warning, TEXT("%s"), *AnimationText);
			BuildingFlipbooks[i * NumBuildingTiers + j] = LoadObject<UPaperFlipbook>(
				nullptr,
				*(AnimationText),
				nullptr,
				LOAD_None,
				nullptr
			);
		}
	}
}

void ABuildingEnvironment::InitializeFlipbooks()
{
	BuildingFlipbooks.Empty();
	for (int i = 0; i < NumBuildingTiers; i++)
	{
		for (int j = 0; j < NumBuildingAssets; j++)
		{
			BuildingFlipbooks.Push(TObjectPtr<UPaperFlipbook>());
		}
	}
}

void ABuildingEnvironment::SetFlipbooks()
{
	int32 StyleNum = FMath::RandRange(0, NumBuildingAssets-1);
	GetSprite()->SetFlipbook(BuildingFlipbooks[StyleNum]);
	
	StyleNum = FMath::RandRange(0, NumBuildingAssets-1);
	Tier1Sprite->SetFlipbook(BuildingFlipbooks[NumBuildingTiers + StyleNum]);
	
	StyleNum = FMath::RandRange(0, NumBuildingAssets-1);
	Tier2Sprite->SetFlipbook(BuildingFlipbooks[(2 * NumBuildingTiers) + StyleNum]);
}
