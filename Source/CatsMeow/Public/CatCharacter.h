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
	// FUNCTIONS
	ACatCharacter();

	/** Returns Accessory Sprite SubObject **/
	FORCEINLINE class UPaperFlipbookComponent* GetAccessorySprite() const { return AccessorySprite; }

	// VARIABLES
private:
	// FUNCTIONS
	void InitializeFlipbooks();
	
	// VARIABLES
	// Accessory Sprite
	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UPaperFlipbookComponent> AccessorySprite;
protected:
	// FUNCTIONS
	// Calculates the current direction of the Character
	virtual void SetCurrentAnimationDirection(FVector const& Velocity, TOptional<FMinimalViewInfo> ViewInfo);

	// Animate function to actually Animate the character using Direction and CatAnimationFlipbooks
	UFUNCTION(BlueprintCallable, Category = "CatCharacter|Animation")
	virtual void Animate(float DeltaTime, FVector OldLocation, FVector const OldVelocity);

	// Assign Flipbooks based on direction
	virtual void AssignFlipbooks(FVector const OldVelocity);
	
	// Updating CatAnimationFlipbooks based on Cat Attributes (Body Shape and Body Texture)
	void LoadCatAnimationFlipbooks(int32 BodyShapeNumber, int32 TextureNumber);

	// Updating CatAccessoryFlipbooks based on Cat Attributes (Body Shape and Body Texture)
	void LoadCatAccessoryFlipbooks(int32 AccessoryNumber);

	// Check if BodyShapeNumber and Texture Index is in CatAnimationFlipbooks Array bounds
	bool CheckIndex(int32 BodyShapeNo, int32 TextureNo) const;

	// Check if Accessory Index is in CatAccessoryFlipbooks Array bounds
	bool CheckAccessoryNumber(int32 AccessoryNo) const;

	// VARIABLES
	// Current Cat Face Direction
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CatCharacter|Config")
	ECatFaceDirection CurrentCatFaceDirection = ECatFaceDirection::Down;

	// Vector used to adjust the cat's direction when not moving
	FVector CatRight = FVector();
	// Rotator to store Temp Camera rotation used to adjust the cat's direction when not moving
	FRotator TempCatRotation = FRotator();
	// Temp Face Direction to adjust the cat's direction when not moving
	ECatFaceDirection TempCatFaceDirection = ECatFaceDirection::Down;

	// Flipbooks for Storing Animations of 2D Sprites
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CatCharacter|Config")
	TArray<FCatFaceFlipbooks> CatAnimationFlipbooks;

	// Flipbooks for Storing Animations of 2D Sprites
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CatCharacter|Config")
	TArray<FCatFaceFlipbooks> CatAccessoryFlipbooks;

	// Calculate Character isMoving or not
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CatCharacter|Config")
	uint8 bIsMoving : 1;

	int32 MaxTextures = 2;
	int32 MaxBodyShapes = 2;
	int32 MaxAccessories = 2;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CatCharacter|Config")
	int32 CatTexture = 0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CatCharacter|Config")
	int32 CatBodyShape = 0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CatCharacter|Config")
	int32 CatAccessory = 0;
};
