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
}

UPCGComponent::~UPCGComponent()
{
	DestroySpawnedActors();
}


// Called when the game starts
void UPCGComponent::BeginPlay()
{
	Super::BeginPlay();
	ParentVolumeRef = Cast<APCGVolume>(GetOwner());
	CreateGrid();
	ProcedurallyGenerateActors(0);
}

void UPCGComponent::ProcedurallyGenerateActors(int32 MapIndex)
{
	for (auto ActorData: ActorSpawnData)
	{
		for (int SpawnRegion = 0; SpawnRegion < ActorData.SpawnRegions.Num(); SpawnRegion++) 
		{
			TSet<int32> Indices = TSet<int32>();
			const int32 MinSpawnX = ActorData.SpawnRegions[SpawnRegion].SpawnMinIndexRangeX < 0 ?
				0 : ActorData.SpawnRegions[SpawnRegion].SpawnMinIndexRangeX;
			const int32 MinSpawnY = ActorData.SpawnRegions[SpawnRegion].SpawnMinIndexRangeY < 0 ?
				0 : ActorData.SpawnRegions[SpawnRegion].SpawnMinIndexRangeY;
			const int32 MaxSpawnX = ActorData.SpawnRegions[SpawnRegion].SpawnMaxIndexRangeX > GridDivisionsX - 1 ?
				0 : ActorData.SpawnRegions[SpawnRegion].SpawnMaxIndexRangeX;
			const int32 MaxSpawnY = ActorData.SpawnRegions[SpawnRegion].SpawnMaxIndexRangeY > GridDivisionsY - 1 ?
				0 : ActorData.SpawnRegions[SpawnRegion].SpawnMaxIndexRangeY;

			for (int i = MinSpawnX; i <= MaxSpawnX; i++)
			{
				for (int j = MinSpawnY; j <= MaxSpawnY; j++)
				{
					const int32 TempIndex = i * GridDivisionsX + j;
					if (!SpawnedIndices.Contains(TempIndex))
					{
						Indices.Add(TempIndex);
					}
				}
			}
			for (int i = 0; i < ActorData.NoOfActors[SpawnRegion]; i++)
			{
				if (Indices.Num() == 0)
				{
					break;
				}
				
				const int32 SpawnInd = Indices.Array()[FMath::RandRange(0, Indices.Num()-1)];

				// UE_LOG(LogTemp, Warning, TEXT("Actor: %s | ActorNo: %d | Position: %d"),
				// 	*ActorData.SpawnActorType->GetName(), i, SpawnInd);

				ActorData.SpawnIndices.Add(SpawnInd);
				SpawnedIndices.Add(SpawnInd);
				Indices.Remove(SpawnInd);
			}
		}
		SpawnActorsAtPoints(ActorData);
	}
	
}

void UPCGComponent::SpawnActorsAtPoints(const FProceduralVolumeSpawnData& ActorData)
{
	for (int i = 0; i < ActorData.SpawnIndices.Num(); i++)
	{
		FActorSpawnParameters SpawnInfo;

		const FRotator SpawnRotation = FRotator(
			0.0f,
			FMath::FRandRange(ActorData.SpawnMinRotation.Yaw, ActorData.SpawnMaxRotation.Yaw),
			0.0f
		);

		const float SpawnScale = FMath::FRandRange(ActorData.SpawnMinScale, ActorData.SpawnMaxScale);
		
		FTransform SpawnActorTransform = FTransform(
			SpawnRotation,
			SpawnPoints[ActorData.SpawnIndices.Array()[i]],
			FVector(SpawnScale)
		);
		AActor* SpawnedActor = GetWorld()->SpawnActor(
			ActorData.SpawnActorType,
			&SpawnActorTransform,
			SpawnInfo);
		SpawnedActor->SetActorEnableCollision(true);
		SpawnedActors.Add(SpawnedActor);
	}
}

void UPCGComponent::DestroySpawnedActors()
{
	SpawnedActors.Empty();
}

void UPCGComponent::CreateGrid()
{
	bEveryTick = false;
	if (ParentVolumeRef)
	{
		const FVector VolumeBoundsMin = ParentVolumeRef->GetComponentsBoundingBox().Min;
		const FVector VolumeBoundsMax = ParentVolumeRef->GetComponentsBoundingBox().Max;
		const FVector VolumeBoundsExtent = ParentVolumeRef->GetComponentsBoundingBox().GetExtent();

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

				FVector SpawnPoint = LineTraceProjectPoint(
					FVector(XRandPos, YRandPos, VolumeBoundsMax.Z),
					FVector(XRandPos, YRandPos, VolumeBoundsMin.Z)
				);
				SpawnPoints.Add(FVector(XRandPos, YRandPos, VolumeBoundsMax.Z));
				
				if (bDebugRun)
				{
					DrawDebugBox(
						GetWorld(),
						FVector(XPos, YPos, VolumeBoundsMax.Z),
						FVector(XExtent, YExtent, 2.0f),
						FColor::Green, !bEveryTick, -1, 0, 1
					);
					DrawDebugPoint(
						GetWorld(), SpawnPoint, 3, FColor::Blue, !bEveryTick, -1, 0
					);
				}

			}
		}
	}
}

FVector UPCGComponent::LineTraceProjectPoint(const FVector& PointToProject, const FVector& EndVector)
{
	FHitResult ProjectHit;

	bool bHitResult = GetWorld()->LineTraceSingleByChannel(
		ProjectHit,
		PointToProject,
		EndVector,
		ECollisionChannel::ECC_Visibility
	);

	if (bHitResult)
	{
		return ProjectHit.Location;
	}
	return EndVector;
}

// Called every frame
void UPCGComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	

}

