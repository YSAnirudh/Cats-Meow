// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PCGComponent.generated.h"

class APCGVolume;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CATSMEOW_API UPCGComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// FUNCTIONS
	// Sets default values for this component's properties
	UPCGComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// VARIABLES

protected:
	// FUNCTIONS
	// Called when the game starts
	virtual void BeginPlay() override;

	// VARIABLES
private:
	// FUNCTIONS
	UFUNCTION(BlueprintCallable, Category="PCGSystem|Generation")
	void GeneratePoints();

	UFUNCTION(BlueprintCallable, Category="PCGSystem|Generation")
	void CreateGrid();

	// VARIABLES
	// Reference to the PCGVolume
	TObjectPtr<APCGVolume> ParentVolumeRef = nullptr;

	// Array holding object positions
	TArray<FVector> SpawnPoints;
	TArray<float> SpawnScale;
	TArray<bool> SpawnedIndices;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="PCGSystem|SpawnSettings", meta = (AllowPrivateAccess="true"))
	TMap<AActor*, int32> ActorsToSpawn;

	// Grid Divisions for generating points
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="PCGSystem|GridSettings", meta = (AllowPrivateAccess="true"))
	int32 GridDivisionsX = 10;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="PCGSystem|GridSettings", meta = (AllowPrivateAccess="true"))
	int32 GridDivisionsY = 10;

	float PaddingMax = 0.0f;

	// Padding Between GridCells
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="PCGSystem|GridSettings", meta = (AllowPrivateAccess="true",
		ClampMin = 0.0f, ClampMax = 1.f, UIMin = 0.0f, UIMax = 1.f))
	float GridPaddingX = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="PCGSystem|GridSettings", meta = (AllowPrivateAccess="true",
		ClampMin = 0.0f, ClampMax = 1.0f, UIMin = 0.0f, UIMax = 1.f))
	float GridPaddingY = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="PCGSystem|SpawnSettings", meta = (AllowPrivateAccess="true"))
	float ScaleMin = 0.5f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="PCGSystem|SpawnSettings", meta = (AllowPrivateAccess="true"))
	float ScaleMax = 2.0f;

	// Bool to use in functions to define if they run every tick or not
	bool bEveryTick = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="PCGSystem|GridSettings", meta = (AllowPrivateAccess="true"))
	bool bDebugRun = true;
};
