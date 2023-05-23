// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTT_FindRandomLocation.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTT_FindRandomLocation::UBTT_FindRandomLocation()
{
	NodeName = TEXT("Find Random Location");

	BlackboardKey.AddVectorFilter(this,
		GET_MEMBER_NAME_CHECKED(UBTT_FindRandomLocation, BlackboardKey));
}

EBTNodeResult::Type UBTT_FindRandomLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	FNavLocation Location;

	AAIController* AIController = OwnerComp.GetAIOwner();
	const APawn* AIPawn = AIController->GetPawn();

	if (AIPawn)
	{
		const FVector Origin = AIPawn->GetActorLocation();
		const UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
		if (NavSystem && NavSystem->GetRandomPointInNavigableRadius(Origin, SearchRadius, Location))
		{
			if (AIController)
			{
				AIController->GetBlackboardComponent()->SetValueAsVector(BlackboardKey.SelectedKeyName, Location.Location);
			}
		}
	}
	
	// DrawDebugSphere(GetWorld(),
	// 	Location.Location,
	// 	20.f, 8, FColor::Red, false, 2.f, 0, 2);
	
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}

FString UBTT_FindRandomLocation::GetStaticDescription() const
{
	return FString::Printf(TEXT("Vector: %s"), *BlackboardKey.SelectedKeyName.ToString());
}
