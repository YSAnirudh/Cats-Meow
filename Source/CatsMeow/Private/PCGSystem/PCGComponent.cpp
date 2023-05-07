// Fill out your copyright notice in the Description page of Project Settings.


#include "PCGSystem/PCGComponent.h"

#include "PCGSystem/PCGVolume.h"

// Sets default values for this component's properties
UPCGComponent::UPCGComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	SpawnPoints.Empty();
	SpawnScale.Empty();
}


// Called when the game starts
void UPCGComponent::BeginPlay()
{
	Super::BeginPlay();
	ParentVolumeRef = Cast<APCGVolume>(GetOwner());
	CreateGrid();
}

void UPCGComponent::GeneratePoints()
{
	
}

void UPCGComponent::CreateGrid()
{
	bEveryTick = false;
	bool bDrawPoint = true;
	if (ParentVolumeRef)
	{
		FVector VolumeBoundsMin = ParentVolumeRef->GetComponentsBoundingBox().Min;
		FVector VolumeBoundsMax = ParentVolumeRef->GetComponentsBoundingBox().Max;
		FVector VolumeBoundsExtent = ParentVolumeRef->GetComponentsBoundingBox().GetExtent();

		if (bDebugRun)
		{
			DrawDebugBox(
				GetWorld(),
				ParentVolumeRef->GetActorLocation(),
				ParentVolumeRef->GetComponentsBoundingBox().GetExtent(),
				FColor::Red,
				!bEveryTick,
				-1,
				0,
				3);
		}

		const float GridSizeX = VolumeBoundsExtent.X * 2.0f / GridDivisionsX;
		const float GridSizeY = VolumeBoundsExtent.Y * 2.0f / GridDivisionsY;
		
		for (int i = 0; i < GridDivisionsX; i++)
		{
			for (int j = 0; j < GridDivisionsY; j++)
			{
				const float XPos = VolumeBoundsMin.X + (GridSizeX * i) + GridSizeX / 2;
				const float YPos = VolumeBoundsMin.Y + (GridSizeY * j) + GridSizeY / 2;
				const float XExtent = GridSizeX / 2 - (GridPaddingX * GridSizeX / 2);
				const float YExtent = GridSizeY / 2 - (GridPaddingY * GridSizeY / 2);
				const float XRandPos = FMath::RandRange(XPos - XExtent, XPos + XExtent);
				const float YRandPos = FMath::RandRange(YPos - YExtent, YPos + YExtent);
				const float RandScale = FMath::RandRange(ScaleMin, ScaleMax);
				SpawnPoints.Add(FVector(XRandPos, YRandPos, VolumeBoundsMin.Z));
				SpawnScale.Add(RandScale);
				SpawnedIndices.Add(false);
				if (bDebugRun)
				{
					DrawDebugBox(
						GetWorld(),
						FVector(XPos, YPos, VolumeBoundsMax.Z),
						FVector(XExtent, YExtent, 2.0f),
						FColor::Green,
						!bEveryTick,
						-1,
						0,
						1
					);
					
					if (bDrawPoint)
					{
						DrawDebugPoint(
							GetWorld(),
							FVector(XRandPos, YRandPos, VolumeBoundsMax.Z),
							1,
							FColor::Blue,
							!bEveryTick,
							-1,
							0
						);
					}
				}
			}
		}
	}
}


// Called every frame
void UPCGComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	

}

