// Fill out your copyright notice in the Description page of Project Settings.


#include "CatCharacter.h"

#include "Kismet/KismetMathLibrary.h"
#include "Paper2D/Classes/PaperFlipbookComponent.h"


ACatCharacter::ACatCharacter()
{
	AccessorySprite = CreateDefaultSubobject<UPaperFlipbookComponent>("AccessorySprite");
	AccessorySprite->SetupAttachment(GetSprite());

	// Call Animate function when the player moves.
	OnCharacterMovementUpdated.AddDynamic(this, &ACatCharacter::Animate);
	
	// Set Absolute Rotation and Initialize the default Flipbook.
	GetSprite()->SetUsingAbsoluteRotation(true);

	bIsMoving = false;

	InitializeFlipbooks();
}

void ACatCharacter::InitializeFlipbooks()
{
	CatAnimationFlipbooks.Empty();
	CatAccessoryFlipbooks.Empty();
	
	// Pushing Arrays to Load Flipbooks Later
	for (int i = 0; i < MaxTextures + MaxBodyShapes; i++)
	{
		CatAnimationFlipbooks.Push(FCatFaceFlipbooks());
	}
	for (int i = 0; i < MaxAccessories; i++)
	{
		CatAccessoryFlipbooks.Push(FCatFaceFlipbooks());
	}

	// Checking Current Index and Setting a BaseFlipbook at the Start
	if (CheckIndex(CatBodyShape, CatTexture))
	{
		GetSprite()->SetFlipbook(CatAnimationFlipbooks[CatTexture * MaxBodyShapes + CatBodyShape].IdleDown);
	}
	if (CheckAccessoryNumber(CatAccessory))
	{
		GetAccessorySprite()->SetFlipbook(CatAccessoryFlipbooks[CatAccessory].IdleDown);
	}
}

void ACatCharacter::SetCurrentAnimationDirection(FVector const& Velocity, TOptional<FMinimalViewInfo> ViewInfo)
{
	// ViewInfo is set, continue
	if (!ViewInfo.IsSet()) return;

	// Calculate forward and Right vector of the character from the ViewInfo
	const FVector Forward = UKismetMathLibrary::GetForwardVector(ViewInfo.GetValue().Rotation);
	const FVector Right = UKismetMathLibrary::GetRightVector(ViewInfo.GetValue().Rotation);

	// Forward Speed and Right Speed indicate the
	// amount of similarity between Velocity and
	// the Forward and Right Vector (Dot Product)
	const float ForwardSpeed = FMath::Floor(FVector::DotProduct(Velocity.GetSafeNormal(), Forward) * 100) / 100;
	const float RightSpeed = FMath::Floor(FVector::DotProduct(Velocity.GetSafeNormal(), Right) * 100) / 100;

	// Temp Log
	// UE_LOG(LogTemp, Warning, TEXT("Forward: %.2f, Right: %.2f"), ForwardSpeed, RightSpeed);

	// Set is moving by checking the character speed
	bIsMoving = RightSpeed != 0.0f || ForwardSpeed != 0.0f;

	// If the Character is moving, only then update Direction.
	if (bIsMoving)
	{
		if (ForwardSpeed > 0.0f && abs(RightSpeed) < 0.25f)
		{
			CurrentCatFaceDirection = ECatFaceDirection::Up;
		}
		else if (ForwardSpeed > 0.5f && RightSpeed >= 0.25f)
		{
			CurrentCatFaceDirection = ECatFaceDirection::UpRight;
		}
		else if (ForwardSpeed > 0.5f && RightSpeed <= -0.25f)
		{
			CurrentCatFaceDirection = ECatFaceDirection::UpLeft;
		}
		else if (ForwardSpeed < 0.5f && abs(RightSpeed) <= 0.25f)
		{
			CurrentCatFaceDirection = ECatFaceDirection::Down;
		}
		else if (ForwardSpeed < -0.5f && RightSpeed >= 0.25f)
		{
			CurrentCatFaceDirection = ECatFaceDirection::DownRight;
		}
		else if (ForwardSpeed < -0.5f && RightSpeed <= -0.25f)
		{
			CurrentCatFaceDirection = ECatFaceDirection::DownLeft;
		}
		else if (abs(ForwardSpeed) < 0.5f && RightSpeed > 0.0f)
		{
			CurrentCatFaceDirection = ECatFaceDirection::Right;
		}
		else
		{
			CurrentCatFaceDirection = ECatFaceDirection::Left;
		}
	}
}

void ACatCharacter::Animate(float DeltaTime, FVector OldLocation, FVector const OldVelocity)
{
	TOptional<FMinimalViewInfo> ViewInfo;
	// If it is the player in control, take the camera into account and calculate the ViewInfo
	if (IsPlayerControlled())
	{
		if (GetWorld())
		{
			APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
			if (PlayerController)
			{
				ACharacter* Character = PlayerController->GetCharacter();
				if (Character)
				{
					Character->CalcCamera(DeltaTime, ViewInfo.Emplace());
				}
			}
		}
	}

	// Set Direction Based on ViewInfo
	SetCurrentAnimationDirection(OldVelocity, ViewInfo);

	// Assign CatAnimationFlipbooks based on direction
	AssignFlipbooks(OldVelocity);
}

void ACatCharacter::AssignFlipbooks(FVector const OldVelocity)
{
	if (!CheckIndex(CatBodyShape, CatTexture))
	{
		return;
	}
	
	// Check if Character is moving to play Idle/Walk Animations
	if (OldVelocity.Size() > 0.0f)
	{
		switch (CurrentCatFaceDirection) {
		case ECatFaceDirection::Up:
			GetSprite()->SetFlipbook(CatAnimationFlipbooks[CatTexture * MaxBodyShapes + CatBodyShape].WalkUp);
			GetAccessorySprite()->SetFlipbook(CatAccessoryFlipbooks[CatAccessory].WalkUp);
			break;
		case ECatFaceDirection::Down:
			GetSprite()->SetFlipbook(CatAnimationFlipbooks[CatTexture * MaxBodyShapes + CatBodyShape].WalkDown);
			GetAccessorySprite()->SetFlipbook(CatAccessoryFlipbooks[CatAccessory].WalkDown);
			break;
		case ECatFaceDirection::Left:
			GetSprite()->SetFlipbook(CatAnimationFlipbooks[CatTexture * MaxBodyShapes + CatBodyShape].WalkLeft);
			GetAccessorySprite()->SetFlipbook(CatAccessoryFlipbooks[CatAccessory].WalkLeft);
			break;
		case ECatFaceDirection::Right:
			GetSprite()->SetFlipbook(CatAnimationFlipbooks[CatTexture * MaxBodyShapes + CatBodyShape].WalkRight);
			GetAccessorySprite()->SetFlipbook(CatAccessoryFlipbooks[CatAccessory].WalkRight);
			break;
		case ECatFaceDirection::UpLeft:
			GetSprite()->SetFlipbook(CatAnimationFlipbooks[CatTexture * MaxBodyShapes + CatBodyShape].WalkLeft);
			GetAccessorySprite()->SetFlipbook(CatAccessoryFlipbooks[CatAccessory].WalkLeft);
			break;
		case ECatFaceDirection::UpRight:
			GetSprite()->SetFlipbook(CatAnimationFlipbooks[CatTexture * MaxBodyShapes + CatBodyShape].WalkRight);
			GetAccessorySprite()->SetFlipbook(CatAccessoryFlipbooks[CatAccessory].WalkRight);
			break;
		case ECatFaceDirection::DownLeft:
			GetSprite()->SetFlipbook(CatAnimationFlipbooks[CatTexture * MaxBodyShapes + CatBodyShape].WalkLeft);
			GetAccessorySprite()->SetFlipbook(CatAccessoryFlipbooks[CatAccessory].WalkLeft);
			break;
		case ECatFaceDirection::DownRight:
			GetSprite()->SetFlipbook(CatAnimationFlipbooks[CatTexture * MaxBodyShapes + CatBodyShape].WalkRight);
			GetAccessorySprite()->SetFlipbook(CatAccessoryFlipbooks[CatAccessory].WalkRight);
			break;
		default: ;
		}
	}
	else
	{
		switch (CurrentCatFaceDirection) {
		case ECatFaceDirection::Up:
			GetSprite()->SetFlipbook(CatAnimationFlipbooks[CatTexture * MaxBodyShapes + CatBodyShape].IdleUp);
			GetAccessorySprite()->SetFlipbook(CatAccessoryFlipbooks[CatAccessory].IdleUp);
			break;
		case ECatFaceDirection::Down:
			GetSprite()->SetFlipbook(CatAnimationFlipbooks[CatTexture * MaxBodyShapes + CatBodyShape].IdleDown);
			GetAccessorySprite()->SetFlipbook(CatAccessoryFlipbooks[CatAccessory].IdleDown);
			break;
		case ECatFaceDirection::Left:
			GetSprite()->SetFlipbook(CatAnimationFlipbooks[CatTexture * MaxBodyShapes + CatBodyShape].IdleLeft);
			GetAccessorySprite()->SetFlipbook(CatAccessoryFlipbooks[CatAccessory].IdleLeft);
			break;
		case ECatFaceDirection::Right:
			GetSprite()->SetFlipbook(CatAnimationFlipbooks[CatTexture * MaxBodyShapes + CatBodyShape].IdleRight);
			GetAccessorySprite()->SetFlipbook(CatAccessoryFlipbooks[CatAccessory].IdleRight);
			break;
		case ECatFaceDirection::UpLeft:
			GetSprite()->SetFlipbook(CatAnimationFlipbooks[CatTexture * MaxBodyShapes + CatBodyShape].IdleLeft);
			GetAccessorySprite()->SetFlipbook(CatAccessoryFlipbooks[CatAccessory].IdleLeft);
			break;
		case ECatFaceDirection::UpRight:
			GetSprite()->SetFlipbook(CatAnimationFlipbooks[CatTexture * MaxBodyShapes + CatBodyShape].IdleRight);
			GetAccessorySprite()->SetFlipbook(CatAccessoryFlipbooks[CatAccessory].IdleRight);
			break;
		case ECatFaceDirection::DownLeft:
			GetSprite()->SetFlipbook(CatAnimationFlipbooks[CatTexture * MaxBodyShapes + CatBodyShape].IdleLeft);
			GetAccessorySprite()->SetFlipbook(CatAccessoryFlipbooks[CatAccessory].IdleLeft);
			break;
		case ECatFaceDirection::DownRight:
			GetSprite()->SetFlipbook(CatAnimationFlipbooks[CatTexture * MaxBodyShapes + CatBodyShape].IdleRight);
			GetAccessorySprite()->SetFlipbook(CatAccessoryFlipbooks[CatAccessory].IdleRight);
			break;
		default: ;
		}
	}
}

void ACatCharacter::LoadCatAnimationFlipbooks(int32 BodyShapeNumber, int32 TextureNumber)
{
	if (!CheckIndex(BodyShapeNumber, TextureNumber))
	{
		return;
	}

	// Get the asset from the Asset Browser and set the value for the flipbooks
	// String based on BodyShape and Texture Number
	const FString AnimationText =
		FString("'/Game/Assets/CatAnimFlipbooks/Flipbooks/c_b") +
		FString::FromInt(BodyShapeNumber) + FString("_t") + FString::FromInt(TextureNumber);

	// Idle Up
	CatAnimationFlipbooks[TextureNumber * MaxBodyShapes + BodyShapeNumber].IdleUp = LoadObject<UPaperFlipbook>(
		nullptr,
		*(AnimationText + FString("/FB_IdleUp.FB_IdleUp'")),
		nullptr,
		LOAD_None,
		nullptr
		);
	// Idle Down
	CatAnimationFlipbooks[TextureNumber * MaxBodyShapes + BodyShapeNumber].IdleDown = LoadObject<UPaperFlipbook>(
		nullptr,
		*(AnimationText + FString("/FB_IdleDown.FB_IdleDown'")),
		nullptr,
		LOAD_None,
		nullptr
		);
	// Idle Left
	CatAnimationFlipbooks[TextureNumber * MaxBodyShapes + BodyShapeNumber].IdleLeft = LoadObject<UPaperFlipbook>(
		nullptr,
		*(AnimationText + FString("/FB_IdleLeft.FB_IdleLeft'")),
		nullptr,
		LOAD_None,
		nullptr
		);
	// Idle Right
	CatAnimationFlipbooks[TextureNumber * MaxBodyShapes + BodyShapeNumber].IdleRight = LoadObject<UPaperFlipbook>(
		nullptr,
		*(AnimationText + FString("/FB_IdleRight.FB_IdleRight'")),
		nullptr,
		LOAD_None,
		nullptr
		);
	
	// Walk Up
	CatAnimationFlipbooks[TextureNumber * MaxBodyShapes + BodyShapeNumber].WalkUp = LoadObject<UPaperFlipbook>(
		nullptr,
		*(AnimationText + FString("/FB_WalkUp.FB_WalkUp'")),
		nullptr,
		LOAD_None,
		nullptr
		);
	// Walk Down
	CatAnimationFlipbooks[TextureNumber * MaxBodyShapes + BodyShapeNumber].WalkDown = LoadObject<UPaperFlipbook>(
		nullptr,
		*(AnimationText + FString("/FB_WalkDown.FB_WalkDown'")),
		nullptr,
		LOAD_None,
		nullptr
		);
	// Walk Left
	CatAnimationFlipbooks[TextureNumber * MaxBodyShapes + BodyShapeNumber].WalkLeft = LoadObject<UPaperFlipbook>(
		nullptr,
		*(AnimationText + FString("/FB_WalkLeft.FB_WalkLeft'")),
		nullptr,
		LOAD_None,
		nullptr
		);
	// Walk Right
	CatAnimationFlipbooks[TextureNumber * MaxBodyShapes + BodyShapeNumber].WalkRight = LoadObject<UPaperFlipbook>(
		nullptr,
		*(AnimationText + FString("/FB_WalkRight.FB_WalkRight'")),
		nullptr,
		LOAD_None,
		nullptr
		);
}

void ACatCharacter::LoadCatAccessoryFlipbooks(int32 AccessoryNumber)
{
	if (!CheckAccessoryNumber(AccessoryNumber))
	{
		return;
	}

	// Get the asset from the Asset Browser and set the value for the flipbooks
	// String based on BodyShape and Texture Number
	const FString AnimationText =
		FString("'/Game/Assets/CatAnimFlipbooks/Flipbooks/c_a") +
		FString::FromInt(AccessoryNumber);

	// Idle Up
	CatAccessoryFlipbooks[AccessoryNumber].IdleUp = LoadObject<UPaperFlipbook>(
		nullptr,
		*(AnimationText + FString("/FB_IdleUp.FB_IdleUp'")),
		nullptr,
		LOAD_None,
		nullptr
		);
	// Idle Down
	CatAccessoryFlipbooks[AccessoryNumber].IdleDown = LoadObject<UPaperFlipbook>(
		nullptr,
		*(AnimationText + FString("/FB_IdleDown.FB_IdleDown'")),
		nullptr,
		LOAD_None,
		nullptr
		);
	// Idle Left
	CatAccessoryFlipbooks[AccessoryNumber].IdleLeft = LoadObject<UPaperFlipbook>(
		nullptr,
		*(AnimationText + FString("/FB_IdleLeft.FB_IdleLeft'")),
		nullptr,
		LOAD_None,
		nullptr
		);
	// Idle Right
	CatAccessoryFlipbooks[AccessoryNumber].IdleRight = LoadObject<UPaperFlipbook>(
		nullptr,
		*(AnimationText + FString("/FB_IdleRight.FB_IdleRight'")),
		nullptr,
		LOAD_None,
		nullptr
		);
	
	// Walk Up
	CatAccessoryFlipbooks[AccessoryNumber].WalkUp = LoadObject<UPaperFlipbook>(
		nullptr,
		*(AnimationText + FString("/FB_WalkUp.FB_WalkUp'")),
		nullptr,
		LOAD_None,
		nullptr
		);
	// Walk Down
	CatAccessoryFlipbooks[AccessoryNumber].WalkDown = LoadObject<UPaperFlipbook>(
		nullptr,
		*(AnimationText + FString("/FB_WalkDown.FB_WalkDown'")),
		nullptr,
		LOAD_None,
		nullptr
		);
	// Walk Left
	CatAccessoryFlipbooks[AccessoryNumber].WalkLeft = LoadObject<UPaperFlipbook>(
		nullptr,
		*(AnimationText + FString("/FB_WalkLeft.FB_WalkLeft'")),
		nullptr,
		LOAD_None,
		nullptr
		);
	// Walk Right
	CatAccessoryFlipbooks[AccessoryNumber].WalkRight = LoadObject<UPaperFlipbook>(
		nullptr,
		*(AnimationText + FString("/FB_WalkRight.FB_WalkRight'")),
		nullptr,
		LOAD_None,
		nullptr
		);
}

bool ACatCharacter::CheckIndex(int32 BodyShapeNo, int32 TextureNo) const
{
	if (TextureNo < 0 || TextureNo >= MaxTextures || BodyShapeNo < 0 || BodyShapeNo >= MaxBodyShapes)
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid Texture/Body Shape Number"));
		return false;
	}
	return true;
}

bool ACatCharacter::CheckAccessoryNumber(int32 AccessoryNo) const
{
	if (AccessoryNo < 0 || AccessoryNo >= MaxAccessories)
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid Accessory Number"));
		return false;
	}
	return true;
}