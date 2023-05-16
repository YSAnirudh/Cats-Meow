// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PCGComponent.generated.h"

USTRUCT(BlueprintType)
struct FSpawnRegionData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SpawnMinIndexRangeX;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SpawnMinIndexRangeY;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SpawnMaxIndexRangeX;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SpawnMaxIndexRangeY;
};

USTRUCT(BlueprintType)
struct FProceduralVolumeSpawnData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UClass> SpawnActorType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FSpawnRegionData> SpawnRegions;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int32> NoOfActors;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpawnMinScale;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpawnMaxScale;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator SpawnMinRotation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator SpawnMaxRotation;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TSet<int32> SpawnIndices;
};

class APCGVolume;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CATSMEOW_API UPCGComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// FUNCTIONS
	// Sets default values for this component's properties
	UPCGComponent();

	virtual ~UPCGComponent();
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// VARIABLES
	// Array holding object positions
	TArray<FVector> SpawnPoints;

protected:
	// FUNCTIONS
	// Called when the game starts
	virtual void BeginPlay() override;

	// VARIABLES
private:
	// FUNCTIONS
	UFUNCTION(BlueprintCallable, Category="PCGSystem|Generation")
	void ProcedurallyGenerateActors(int32 MapIndex);

	void SpawnActorsAtPoints(const FProceduralVolumeSpawnData& ActorData);
	void DestroySpawnedActors();

	UFUNCTION(BlueprintCallable, Category="PCGSystem|Generation")
	void CreateGrid();

	UFUNCTION(BlueprintCallable, Category="PCGSystem|Generation")
	FVector LineTraceProjectPoint(const FVector& PointToProject, const FVector& EndVector);

	// VARIABLES
	// Reference to the PCGVolume
	TObjectPtr<APCGVolume> ParentVolumeRef = nullptr;

	TSet<int32> SpawnedIndices;
	TArray<AActor*> SpawnedActors;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="PCGSystem|SpawnSettings", meta = (AllowPrivateAccess="true"))
	TArray<FProceduralVolumeSpawnData> ActorSpawnData;

	// Grid Divisions for generating points
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="PCGSystem|GridSettings", meta = (AllowPrivateAccess="true"))
	int32 GridDivisionsX = 10;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="PCGSystem|GridSettings", meta = (AllowPrivateAccess="true"))
	int32 GridDivisionsY = 10;

	// Padding Between GridCells
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="PCGSystem|GridSettings", meta = (AllowPrivateAccess="true",
		ClampMin = 0.0f, ClampMax = 1.f, UIMin = 0.0f, UIMax = 1.f))
	float GridPaddingX = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="PCGSystem|GridSettings", meta = (AllowPrivateAccess="true",
		ClampMin = 0.0f, ClampMax = 1.0f, UIMin = 0.0f, UIMax = 1.f))
	float GridPaddingY = 0.f;

	// Bool to use in functions to define if they run every tick or not
	bool bEveryTick = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="PCGSystem|GridSettings", meta = (AllowPrivateAccess="true"))
	bool bDebugRun = true;
};
