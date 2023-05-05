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
	// FUNCTIONS
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
	
	virtual void Tick(float DeltaSeconds) override;
	
	// VARIABLES
private:
	// FUNCTIONS
	void LoadFlipbooks();
	
	// VARIABLES
	// Spring arm and Camera
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* CameraComponent;
	
protected:
	// FUNCTIONS
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	// Code for aligning character to the camera
	void AlignCharacterToCamera() const;
	
	// Clamping Camera rotation between CameraPitchMax and CameraPitchMin
	void ClampCameraPitch() const;
	
	// Calculates the current direction of the Character
	virtual void SetCurrentAnimationDirection(FVector const& Velocity, TOptional<FMinimalViewInfo> ViewInfo) override;

    // Animate function to actually Animate the character using Direction and CatAnimationFlipbooks
	virtual void Animate(float DeltaTime, FVector OldLocation, FVector const OldVelocity) override;

	// VARIABLES
	// The amount by which the cat sprite tilts
	// when moving in (UpLeft, UpRight, DownRight, DownLeft)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	float SpriteTiltYaw = 40.f;

	// Pitch clamp for camera.
	float CameraPitchMax = 50.f;
	float CameraPitchMin = 10.f;
};


