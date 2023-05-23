// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/CatAIController.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

ACatAIController::ACatAIController()
{
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Cat Blackboard Component"));
	check(BlackboardComponent);

	BehaviourTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("Cat Behavior Tree Component"));
	check(BehaviourTreeComponent);
}

void ACatAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (!InPawn)
	{
		return;
	}

	if (Blackboard && BehaviourTree)
	{
		Blackboard->InitializeBlackboard(*BehaviourTree->BlackboardAsset);
	}
}

void ACatAIController::BeginPlay()
{
	Super::BeginPlay();

	if (BehaviourTree && BehaviourTreeComponent)
	{
		RunBehaviorTree(BehaviourTree);
		BehaviourTreeComponent->StartTree(*BehaviourTree);
	}
}
