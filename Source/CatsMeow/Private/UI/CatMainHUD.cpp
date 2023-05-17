// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CatMainHUD.h"

#include "Blueprint/WidgetBlueprintLibrary.h"

ACatMainHUD::ACatMainHUD()
{
}

void ACatMainHUD::DrawHUD()
{
	Super::DrawHUD();
	
}

void ACatMainHUD::BeginPlay()
{
	Super::BeginPlay();
	UWidgetBlueprintLibrary::SetInputMode_GameOnly(GetOwningPlayerController());
	if (!CatCustomizationMenuWidgetInstance && CatCustomizationMenuWidget)
	{
		CatCustomizationMenuWidgetInstance = CreateWidget<UUserWidget>(GetWorld(), CatCustomizationMenuWidget);
		if (CatCustomizationMenuWidgetInstance)
		{
			CatCustomizationMenuWidgetInstance->AddToViewport();
		}
	}

	if (!StatBarWidgetInstance && StatBarWidget)
	{
		StatBarWidgetInstance = CreateWidget<UUserWidget>(GetWorld(), StatBarWidget);
		if (StatBarWidgetInstance)
		{
			StatBarWidgetInstance->SetIsEnabled(true);
			StatBarWidgetInstance->SetVisibility(ESlateVisibility::Visible);
			WidgetList.Add(StatBarWidgetInstance);
		}
	}

	for (auto Widget: WidgetList)
	{
		if (Widget)
		{
			Widget->AddToViewport();
		}
	}
}

void ACatMainHUD::ShowStatBar()
{
	if (StatBarWidgetInstance)
	{
		StatBarWidgetInstance->SetIsEnabled(true);
		StatBarWidgetInstance->SetVisibility(ESlateVisibility::Visible);
	}
}

void ACatMainHUD::HideStatBar()
{
	if (StatBarWidgetInstance)
	{
		StatBarWidgetInstance->SetIsEnabled(false);
		StatBarWidgetInstance->SetVisibility(ESlateVisibility::Hidden);
	}
}

void ACatMainHUD::ShowCatCustomizationScreen()
{
	if (CatCustomizationMenuWidgetInstance)
	{
		CatCustomizationMenuWidgetInstance->SetIsEnabled(true);
		CatCustomizationMenuWidgetInstance->SetVisibility(ESlateVisibility::Visible);
	}
}

void ACatMainHUD::HideCatCustomizationScreen()
{
	if (CatCustomizationMenuWidgetInstance)
	{
		CatCustomizationMenuWidgetInstance->SetIsEnabled(false);
		CatCustomizationMenuWidgetInstance->SetVisibility(ESlateVisibility::Hidden);
	}
}
