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
	GenerateIndices();
	SpawnActorsAtPoints();
}

void UPCGComponent::GenerateIndices()
{
	int32 NoOfIndices = 0;
	for (const auto ActorData: ActorsToSpawn)
	{
		NoOfIndices += ActorData.Value;
	}
	TSet<int32> Indices = TSet<int32>();
	for (int i = 0; i < GridDivisionsX * GridDivisionsY; i++)
	{
		Indices.Add(i);
	}
	for (int i = 0; i < NoOfIndices; i++)
	{
		if (Indices.Num() == 0)
		{
			break;
		}
		// Generate Logic (FILTER FUNCTIONALITY HERE!!!)
		const int32 RandomInt = FMath::RandRange(0, Indices.Num()-1);
		
		//UE_LOG(LogTemp, Warning, TEXT("%d"), *Indices.begin() + RandomInt);
		SpawnIndices.Add(Indices.Array()[RandomInt]);
		Indices.Remove(Indices.Array()[RandomInt]);
	}
}

void UPCGComponent::SpawnActorsAtPoints()
{
	TArray<TObjectPtr<UClass>> TempActorArray;
	ActorsToSpawn.GenerateKeyArray(TempActorArray);

	for (int i = 0; i < SpawnIndices.Num(); i++)
	{
		const int32 SpawnIndex = ChooseActorAndDecrementCount();
		//UE_LOG(LogTemp, Warning, TEXT("%d"), SpawnIndex);
		if (SpawnIndex != -1 && SpawnIndex < TempActorArray.Num())
		{
			FActorSpawnParameters SpawnInfo;
			// float ActorHalfHeight = 0.5f * (TempActorArray[SpawnIndex]->GetComponentsBoundingBox().Max.Z -
			// 	TempActorArray[SpawnIndex]->GetComponentsBoundingBox().Min.Z);
			// FVector TempSpawnPoint = FVector(SpawnPoints[i].X, SpawnPoints[i].Y, SpawnPoints[i].Z + 1.0f + ActorHalfHeight);
			AActor* SpawnedActor = GetWorld()->SpawnActor(
				TempActorArray[SpawnIndex],
				&SpawnPoints[SpawnIndices[i]],
				&FRotator::ZeroRotator,
				SpawnInfo);
			SpawnedActor->SetActorEnableCollision(true);
			SpawnedActors.Add(SpawnedActor);
		}
		else
		{
			//UE_LOG(LogTemp, Warning, TEXT("Out of Bounds"));
		}
	}
}

int32 UPCGComponent::ChooseActorAndDecrementCount()
{
	TArray<int32> ActorsIndicesToBeSpawned;
	int32 IndexOfActor = 0;
	
	for (auto const ActorToSpawn : ActorsToSpawn)
	{
		if (ActorToSpawn.Value > 0)
		{
			//UE_LOG(LogTemp, Warning, TEXT("%s: %d"), *ActorToSpawn.Key.Get()->GetName(), ActorToSpawn.Value);
			ActorsIndicesToBeSpawned.Add(IndexOfActor);
		}
		IndexOfActor++;
	}

	IndexOfActor = 0;
	
	// for (int i = 0; i < ActorsIndicesToBeSpawned.Num(); i++) {
	// 	UE_LOG(LogTemp, Warning, TEXT("%d"), ActorsIndicesToBeSpawned[i]);
	// }
	// UE_LOG(LogTemp, Warning, TEXT("------"));
	
	if (ActorsIndicesToBeSpawned.Num() != 0)
	{
		const int32 RandomIndex = FMath::RandRange(0, ActorsIndicesToBeSpawned.Num()-1);
		for (auto const ActorCount : ActorsToSpawn)
		{
			if (ActorsIndicesToBeSpawned[RandomIndex] == IndexOfActor)
			{
				ActorsToSpawn[ActorCount.Key]--;
				return ActorsIndicesToBeSpawned[RandomIndex];
			}
			IndexOfActor++;
		}
		return -1;
	}
	return -1;
}

void UPCGComponent::DestroySpawnedActors()
{
	// for(AActor* SpawnedActor : SpawnedActors)
	// {
	// 	SpawnedActor->Destroy();
	// }
	SpawnedActors.Empty();
}

void UPCGComponent::CreateGrid()
{
	bEveryTick = false;
	if (ParentVolumeRef)
	{
		FVector VolumeBoundsMin = ParentVolumeRef->GetComponentsBoundingBox().Min;
		FVector VolumeBoundsMax = ParentVolumeRef->GetComponentsBoundingBox().Max;
		FVector VolumeBoundsExtent = ParentVolumeRef->GetComponentsBoundingBox().GetExtent();

		if (bDebugRun)
		{
			// DrawDebugBox(
			// 	GetWorld(),
			// 	ParentVolumeRef->GetActorLocation(), ParentVolumeRef->GetComponentsBoundingBox().GetExtent(),
			// 	FColor::Red, !bEveryTick, -1, 0, 3);
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
				float XRandPos = FMath::RandRange(XPos - XExtent, XPos + XExtent);
				float YRandPos = FMath::RandRange(YPos - YExtent, YPos + YExtent);

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

