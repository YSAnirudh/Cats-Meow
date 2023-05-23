// Fill out your copyright notice in the Description page of Project Settings.


#include "Environment/FoodBowl.h"

#include "Kismet/GameplayStatics.h"
#include "UI/MiniGameWidget.h"

AFoodBowl::AFoodBowl()
{
	bIsInteractable = true;
}

void AFoodBowl::MainCharacterInteractFunction()
{
	if (bIsInteractable && !bHasPlayedMiniGame)
	{
		UE_LOG(LogTemp, Warning, TEXT("Food Bowl Mini Game Open!!"));
		if (!IsValid(MiniGameWidget))
		{
			UE_LOG(LogTemp, Warning, TEXT("No Widget"));
			return;
		}
		if (IsValid(MiniGameWidget) && !MiniGameWidget->IsInViewport())
		{
			MiniGameWidget->AddToViewport();
			bHasPlayedMiniGame = true;
			AMainCatCharacter* MainCatRef = Cast<AMainCatCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
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

void AFoodBowl::SetCanInteract(bool bInteract)
{
	if (bIsInteractable)
	{
		bCanInteract = bInteract;
	}
}
