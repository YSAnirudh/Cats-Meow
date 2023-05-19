// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CatCharacter.h"
#include "MainCatCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInteractLogicDelegate, class AMainCatCharacter*, MainCatRef);
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

	void LoadPlayerSelectionFromSlot(FString SlotName);
	void SavePlayerSelectionToSlot(FString SlotName);

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
	
	UFUNCTION()
	void OnInteract();

	// Delegates
	UPROPERTY(BlueprintAssignable)
	FInteractLogicDelegate InteractLogicDelegate;
	
	// INLINE FUNCTIONS
	// INCREMENT AND DECREMENT STATS
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void IncrementHappiness()
	{
		CatHappinessCurrent = CatHappinessCurrent > CatHappinessMax ? CatHappinessMax : CatHappinessCurrent + 1;
	}
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void DecrementHappiness()
	{
		CatHappinessCurrent = CatHappinessCurrent < 0 ? 0 : CatHappinessCurrent - 1;
	}
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void IncrementHunger()
	{
		CatHungerCurrent = CatHungerCurrent > CatHungerMax ? CatHungerMax : CatHungerCurrent + 1;
	}
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void DecrementHunger()
	{
		CatHungerCurrent = CatHungerCurrent < 0 ? 0 : CatHungerCurrent - 1;
	}
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void IncrementHygiene()
	{
		CatHygieneCurrent = CatHygieneCurrent > CatHygieneMax ? CatHygieneMax : CatHygieneCurrent + 1;
	}
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void DecrementHygiene()
	{
		CatHygieneCurrent = CatHygieneCurrent < 0 ? 0 : CatHygieneCurrent - 1;
	}
	UFUNCTION()
	FORCEINLINE class UCameraComponent* GetPlayerCameraComponent() const { return CameraComponent; }
	UFUNCTION()
	FORCEINLINE int32 GetCurrentMap() const { return MapNumber; }
	UFUNCTION()
	FORCEINLINE void SetCurrentMap(int32 MapNO) { this->MapNumber = MapNO; }

	
	// VARIABLES
private:
	// FUNCTIONS
	
	// VARIABLES
	// Spring arm and Camera
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* CameraComponent;

	// Map Number
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CatStats", meta=(AllowPrivateAccess = "true"))
	int32 MapNumber = 0;
	
	// Cat Stats
	// Hunger
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CatStats", meta=(AllowPrivateAccess = "true"))
	int32 CatHungerMax = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CatStats", meta=(AllowPrivateAccess = "true"))
	int32 CatHungerCurrent = 2;

	// Happiness
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CatStats", meta=(AllowPrivateAccess = "true"))
	int32 CatHappinessMax = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CatStats", meta=(AllowPrivateAccess = "true"))
	int32 CatHappinessCurrent = 2;

	// Hygiene
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CatStats", meta=(AllowPrivateAccess = "true"))
	int32 CatHygieneMax = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CatStats", meta=(AllowPrivateAccess = "true"))
	int32 CatHygieneCurrent = 2;
	
protected:
	// FUNCTIONS
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	// Code for aligning character to the camera
	virtual void AlignCharacterToCamera() override;
	
	// Clamping Camera rotation between CameraPitchMax and CameraPitchMin
	void ClampCameraPitch() const;
	
	// Calculates the current direction of the Character
	virtual void SetCurrentAnimationDirection(FVector const& Velocity, TOptional<FMinimalViewInfo> ViewInfo) override;

    // Animate function to actually Animate the character using Direction and CatAnimationFlipbooks
	virtual void Animate(float DeltaTime, FVector OldLocation, FVector const OldVelocity) override;

	// VARIABLES

	// Pitch clamp for camera.
	float CameraPitchMax = 50.f;
	float CameraPitchMin = 10.f;
};

