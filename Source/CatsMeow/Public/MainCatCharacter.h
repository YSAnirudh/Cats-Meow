// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CatCharacter.h"
#include "MainCatCharacter.generated.h"

/**
 * 
 */
UCLASS()
class CATSMEOW_API AMainCatCharacter : public ACatCharacter
{
	GENERATED_BODY()
public:
	AMainCatCharacter();

	// Input Handling Functions
	UFUNCTION()
	void MoveForward(float AxisValue);
	UFUNCTION()
	void MoveRight(float AxisValue);
	UFUNCTION()
	void LookUp(float AxisValue);
	UFUNCTION()
	void Turn(float AxisValue);
	
protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	// Calculates the current direction of the Character
	virtual void SetCurrentAnimationDirection(FVector const& Velocity, TOptional<FMinimalViewInfo> ViewInfo) override;

	// Spring arm and Camera
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UCameraComponent* CameraComponent;
};
