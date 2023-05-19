// Fill out your copyright notice in the Description page of Project Settings.


#include "Environment/FoodBowl.h"

AFoodBowl::AFoodBowl()
{
}

void AFoodBowl::MainCharacterInteractFunction(AMainCatCharacter* MainCatRef)
{
	Super::MainCharacterInteractFunction(MainCatRef);

	if (MainCatRef)
	{
		UE_LOG(LogTemp, Warning, TEXT("Food Bowl Mini Game Open!!"));
		// Open Widget for MiniGame
	}
}
