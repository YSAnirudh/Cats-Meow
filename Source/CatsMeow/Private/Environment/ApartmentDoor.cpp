// Fill out your copyright notice in the Description page of Project Settings.


#include "Environment/ApartmentDoor.h"

#include "Kismet/GameplayStatics.h"

AApartmentDoor::AApartmentDoor()
{
}

void AApartmentDoor::MainCharacterInteractFunction()
{
	AMainCatCharacter* MainCatRef = Cast<AMainCatCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if (!MainCatRef)
	{
		return;
	}	
	if (MainCatRef->GetCurrentMap() == -1)
	{
		const int32 MapToGo = FMath::RandRange(0, 3);

		const FString MapName = FString("MapGeneration") + FString::FromInt(MapToGo + 1);
		switch(MapToGo)
		{
		case 0:
			MainCatRef->SetCurrentMap(0);
			MainCatRef->SavePlayerSelectionToSlot(TEXT("MainSlot"));
			UGameplayStatics::OpenLevel(GetWorld(), *MapName);
			break;
		case 1:
			MainCatRef->SetCurrentMap(1);
			MainCatRef->SavePlayerSelectionToSlot(TEXT("MainSlot"));
			UGameplayStatics::OpenLevel(GetWorld(), *MapName);
			break;
		case 2:
			MainCatRef->SetCurrentMap(2);
			MainCatRef->SavePlayerSelectionToSlot(TEXT("MainSlot"));
			UGameplayStatics::OpenLevel(GetWorld(), *MapName);
			break;
		case 3:
			MainCatRef->SetCurrentMap(3);
			MainCatRef->SavePlayerSelectionToSlot(TEXT("MainSlot"));
			UGameplayStatics::OpenLevel(GetWorld(), *MapName);
			break;
		default:
			break;
		}
		
	}
	else
	{
		MainCatRef->SetCurrentMap(-1);
		MainCatRef->SavePlayerSelectionToSlot(TEXT("MainSlot"));
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
