// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MiniGameWidget.h"

#include "MainCatCharacter.h"
#include "Kismet/GameplayStatics.h"

void UMiniGameWidget::OnMiniGameFinish(bool bCatWon, int32 MiniGameNumber)
{
	AMainCatCharacter* MainCatRef = Cast<AMainCatCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if (MainCatRef)
	{
		MainCatRef->EnableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		if (bCatWon)
		{
			switch (MiniGameNumber)
			{
			case 0:
				MainCatRef->IncrementHygiene();
				break;
			case 1:
				MainCatRef->IncrementHappiness();
				break;
			case 2:
				MainCatRef->IncrementHunger();
				break;
			default:
				break;
			}
		}
	}
	if (IsInViewport())
	{
		RemoveFromViewport();
	}
}
