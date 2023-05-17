// Fill out your copyright notice in the Description page of Project Settings.


#include "PCGSystem/PCGVolume.h"

APCGVolume::APCGVolume()
{
	PCGComponent = CreateDefaultSubobject<UPCGComponent>("PCG Component");
	RootComponent->SetMobility(EComponentMobility::Movable);
}
