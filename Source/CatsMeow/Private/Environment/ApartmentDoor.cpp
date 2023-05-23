// Fill out your copyright notice in the Description page of Project Settings.


#include "Environment/ApartmentDoor.h"

#include "Kismet/GameplayStatics.h"

AApartmentDoor::AApartmentDoor()
{
}

void AApartmentDoor::MainCharacterInteractFunction(AMainCatCharacter* MainCatCharacter)
{
	if (!MainCatCharacter)
	{
		return;
	}	
	if (MainCatCharacter->GetCurrentMap() == -1)
	{
		const int32 MapToGo = FMath::RandRange(0, 3);

		const FString MapName = FString("MapGeneration") + FString::FromInt(MapToGo + 1);
		switch(MapToGo)
		{
		case 0:
			MainCatCharacter->SetCurrentMap(0);
			MainCatCharacter->SavePlayerSelectionToSlot(TEXT("MainSlot"));
			UGameplayStatics::OpenLevel(GetWorld(), *MapName);
			break;
		case 1:
			MainCatCharacter->SetCurrentMap(1);
			MainCatCharacter->SavePlayerSelectionToSlot(TEXT("MainSlot"));
			UGameplayStatics::OpenLevel(GetWorld(), *MapName);
			break;
		case 2:
			MainCatCharacter->SetCurrentMap(2);
			MainCatCharacter->SavePlayerSelectionToSlot(TEXT("MainSlot"));
			UGameplayStatics::OpenLevel(GetWorld(), *MapName);
			break;
		case 3:
			MainCatCharacter->SetCurrentMap(3);
			MainCatCharacter->SavePlayerSelectionToSlot(TEXT("MainSlot"));
			UGameplayStatics::OpenLevel(GetWorld(), *MapName);
			break;
		default:
			break;
		}
		
	}
	else
	{
		MainCatCharacter->SetCurrentMap(-1);
		MainCatCharacter->SavePlayerSelectionToSlot(TEXT("MainSlot"));
		UGameplayStatics::OpenLevel(GetWorld(), TEXT("CatApartment"));
	}
}

void AApartmentDoor::SetCanInteract(bool bInteract)
{
	if (bIsInteractable)
	{
		bCanInteract = bInteract;
	}
}
