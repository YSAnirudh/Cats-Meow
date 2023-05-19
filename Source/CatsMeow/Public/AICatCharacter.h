// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CatCharacter.h"
#include "AICatCharacter.generated.h"

/**
 * 
 */
UCLASS()
class CATSMEOW_API AAICatCharacter : public ACatCharacter
{
	GENERATED_BODY()
public:
	// FUNCTIONS
	AAICatCharacter();

	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool GetCanCharacterInteract() const { return bCanInteract; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE class UBehaviorTree* GetBehaviorTree() const { return BehaviourTree; }

	UFUNCTION()
	void OnStartInteract(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
	void OnEndInteract(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	// VARIABLES
protected:
	// FUNCTIONS
	virtual void BeginPlay() override;
	// VARIABLES
private:
	// FUNCTIONS
	UFUNCTION()
	virtual void MainCharacterInteractFunction(class AMainCatCharacter* MainCatRef);
	
	// VARIABLES
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction", meta = (AllowPrivateAccess = "true"))
	class USphereComponent* InteractionSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction", meta = (AllowPrivateAccess = "true"))
	bool bCanInteract = false;

	// Cat AI Behavior Tree
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	class UBehaviorTree* BehaviourTree;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true", MakeEditWidget = "true"))
	FVector PatrolPoint;

	class ACatAIController* CatAIController = nullptr;
};
