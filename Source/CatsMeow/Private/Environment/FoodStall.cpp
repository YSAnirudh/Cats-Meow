// Fill out your copyright notice in the Description page of Project Settings.


#include "Environment/FoodStall.h"

#include "PaperFlipbookComponent.h"
#include "PaperFlipbook.h"
#include "Kismet/GameplayStatics.h"
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

void AFoodStall::MainCharacterInteractFunction()
{
	if (bIsInteractable && !bHasPlayedMiniGame)
	{
		UE_LOG(LogTemp, Warning, TEXT("Stall Mini Game Open!!"));
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

void AFoodStall::SetCanInteract(bool bInteract)
{
	if (bIsInteractable)
	{
		bCanInteract = bInteract;
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
