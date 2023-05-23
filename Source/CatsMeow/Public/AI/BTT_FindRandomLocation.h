// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTT_FindRandomLocation.generated.h"

/**
 * 
 */
UCLASS()
class CATSMEOW_API UBTT_FindRandomLocation : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	// FUNCTIONS
	UBTT_FindRandomLocation();

	// VARIABLES
protected:
	// FUNCTIONS

	// VARIABLES
private:
	// FUNCTIONS
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual FString GetStaticDescription() const override;
	
	// VARIABLES
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Search", meta = (AllowPrivateAccess = true))
	float SearchRadius = 1000.f;
};
