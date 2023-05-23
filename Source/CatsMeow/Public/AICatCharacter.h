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

	UFUNCTION()
	void OnStartInteract(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
	void OnEndInteract(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION()
	virtual void MainCharacterInteractFunction(class AMainCatCharacter* MainCatCharacter);
	UFUNCTION()
	FORCEINLINE int32 GetCanInteract() const { return bCanInteract; }
	UFUNCTION()
	FORCEINLINE void SetCanInteract(bool bInteract) { this->bCanInteract = bInteract; }
	// VARIABLES
protected:
	// FUNCTIONS
	virtual void BeginPlay() override;
	// VARIABLES
private:
	// FUNCTIONS
	
	// VARIABLES
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction", meta = (AllowPrivateAccess = "true"))
	class USphereComponent* InteractionSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction", meta = (AllowPrivateAccess = "true"))
	bool bCanInteract = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction", meta = (AllowPrivateAccess = "true"))
	bool bHasPlayedMiniGame = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true", MakeEditWidget = "true"))
	FVector PatrolPoint;

	class UMiniGameWidget* MiniGameWidget = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MiniGame", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UMiniGameWidget> MiniGameWidgetClass;
};
