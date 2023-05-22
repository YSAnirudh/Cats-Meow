// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "CatAIController.generated.h"

/**
 * 
 */
UCLASS()
class CATSMEOW_API ACatAIController : public AAIController
{
	GENERATED_BODY()
public:
	// FUNCTIONS
	ACatAIController();

	virtual void OnPossess(APawn* InPawn) override;

	UFUNCTION(BlueprintCallable)
	FORCEINLINE class UBlackboardComponent* GetCatBlackboardComponent() const { return BlackboardComponent; }
	
	// VARIABLES
protected:
	// FUNCTIONS
	virtual void BeginPlay() override;
	
	// VARIABLES
private:
	// FUNCTIONS
	
	// VARIABLES
	// Cat AI Behavior Tree
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	class UBlackboardComponent* BlackboardComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	class UBehaviorTreeComponent* BehaviourTreeComponent;

	// Cat AI Behavior Tree
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	class UBehaviorTree* BehaviourTree;
};
