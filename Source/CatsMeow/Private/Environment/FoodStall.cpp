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
		if (!IsValid(MiniGameWidget))
		{
			UE_LOG(LogTemp, Warning, TEXT("No Widget"));
			return;
		}
		if (IsValid(MiniGameWidget) && !MiniGameWidget->IsInViewport())
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
