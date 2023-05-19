// Fill out your copyright notice in the Description page of Project Settings.


#include "Environment/Rug.h"

ARug::ARug()
{
}

void ARug::MainCharacterInteractFunction(AMainCatCharacter* MainCatRef)
{
	Super::MainCharacterInteractFunction(MainCatRef);

	if (MainCatRef)
	{
		UE_LOG(LogTemp, Warning, TEXT("Rug Mini Game Open!!"));
		// Open Widget for MiniGame
	}
}
