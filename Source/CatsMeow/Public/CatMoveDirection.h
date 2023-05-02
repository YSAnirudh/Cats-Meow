﻿#pragma once

#include "CatMoveDirection.generated.h"

class UPaperFlipbook;

// Cat Face Directions
UENUM(BlueprintType, Category = "Animation")
enum class ECatFaceDirection : uint8
{
	Up UMETA(DisplayName = "Up"),
	Down UMETA(DisplayName = "Down"),
	Left UMETA(DisplayName = "Left"),
	Right UMETA(DisplayName = "Right"),
	UpLeft UMETA(DisplayName = "Up Left"),
	UpRight UMETA(DisplayName = "Up Right"),
	DownLeft UMETA(DisplayName = "Down Left"),
	DownRight UMETA(DisplayName = "Down Right")
};

// Flipbook Storage for Walk and Idle Animations
USTRUCT(BlueprintType, Category = "Animation")
struct CATSMEOW_API FCatFaceFlipbooks
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CatCharacter|CatSprites")
	TObjectPtr<UPaperFlipbook> IdleUp {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CatCharacter|CatSprites")
	TObjectPtr<UPaperFlipbook> IdleDown {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CatCharacter|CatSprites")
	TObjectPtr<UPaperFlipbook> IdleLeft {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CatCharacter|CatSprites")
	TObjectPtr<UPaperFlipbook> IdleRight {nullptr};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CatCharacter|CatSprites")
	TObjectPtr<UPaperFlipbook> IdleUpLeft {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CatCharacter|CatSprites")
	TObjectPtr<UPaperFlipbook> IdleUpRight {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CatCharacter|CatSprites")
	TObjectPtr<UPaperFlipbook> IdleDownLeft {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CatCharacter|CatSprites")
	TObjectPtr<UPaperFlipbook> IdleDownRight {nullptr};



	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CatCharacter|CatSprites")
	TObjectPtr<UPaperFlipbook> WalkUp {nullptr};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CatCharacter|CatSprites")
	TObjectPtr<UPaperFlipbook> WalkDown {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CatCharacter|CatSprites")
	TObjectPtr<UPaperFlipbook> WalkLeft {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CatCharacter|CatSprites")
	TObjectPtr<UPaperFlipbook> WalkRight {nullptr};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CatCharacter|CatSprites")
	TObjectPtr<UPaperFlipbook> WalkUpLeft {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CatCharacter|CatSprites")
	TObjectPtr<UPaperFlipbook> WalkUpRight {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CatCharacter|CatSprites")
	TObjectPtr<UPaperFlipbook> WalkDownLeft {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CatCharacter|CatSprites")
	TObjectPtr<UPaperFlipbook> WalkDownRight {nullptr};

};
