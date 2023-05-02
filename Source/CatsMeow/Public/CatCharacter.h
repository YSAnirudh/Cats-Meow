// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "Paper2D/Classes/PaperFlipbook.h"
#include "CatMoveDirection.h"
#include "CatCharacter.generated.h"

/**
 * 
 */
UCLASS()
class CATSMEOW_API ACatCharacter : public APaperCharacter
{
	GENERATED_BODY()
public:
	ACatCharacter();
protected:
	// Calculates the current direction of the Character
	virtual void SetCurrentAnimationDirection(FVector const& Velocity, TOptional<FMinimalViewInfo> ViewInfo);

	// Animate function to actually Animate the character using Direction and Flipbooks
	UFUNCTION(BlueprintCallable, Category = "CatCharacter|Animation")
	virtual void Animate(float DeltaTime, FVector OldLocation, FVector const OldVelocity);

	// Current Cat Face Direction
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CatCharacter|Config")
	ECatFaceDirection CurrentCatFaceDirection = ECatFaceDirection::Down;

	// Flipbooks for Storing Animations of 2D Sprites
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CatCharacter|Config")
	FCatFaceFlipbooks Flipbooks;

	// Calculate Character isMoving or not
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CatCharacter|Config")
	uint8 bIsMoving : 1;
};
