// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/CatAIController.h"

#include "AICatCharacter.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

ACatAIController::ACatAIController()
{
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Cat Blackboard Component"));
	check(BlackboardComponent);

	BehaviourTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("Behavior Tree Component"));
	check(BehaviourTreeComponent);
}

void ACatAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (!InPawn)
	{
		return;
	}

	AAICatCharacter* AICatCharacter = Cast<AAICatCharacter>(InPawn);
	if (AICatCharacter)
	{
		if (AICatCharacter->GetBehaviorTree())
		{
			BlackboardComponent->InitializeBlackboard(*AICatCharacter->GetBehaviorTree()->BlackboardAsset);
		}
	}
}
