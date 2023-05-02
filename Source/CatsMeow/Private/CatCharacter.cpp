// Fill out your copyright notice in the Description page of Project Settings.


#include "CatCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "Paper2D/Classes/PaperFlipbookComponent.h"


ACatCharacter::ACatCharacter()
{
	// Call Animate function when the player moves.
	OnCharacterMovementUpdated.AddDynamic(this, &ACatCharacter::Animate);

	bIsMoving = false;

	// Set Absolute Rotation and Initialize the default Flipbook.
	GetSprite()->SetUsingAbsoluteRotation(true);
	GetSprite()->SetFlipbook(Flipbooks.IdleDown);
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
	//UE_LOG(LogTemp, Warning, TEXT("Forward: %.2f, Right: %.2f"), ForwardSpeed, RightSpeed);

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
	if (!IsPlayerControlled())
	{
		UWorld* World = GetWorld();
		if (World)
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

	// Check if Character is moving to play Idle/Walk Animations
	if (OldVelocity.Size() > 0.0f)
	{
		switch (CurrentCatFaceDirection) {
			case ECatFaceDirection::Up:
				GetSprite()->SetFlipbook(Flipbooks.WalkUp);
				break;
			case ECatFaceDirection::Down:
				GetSprite()->SetFlipbook(Flipbooks.WalkDown);
				break;
			case ECatFaceDirection::Left:
				GetSprite()->SetFlipbook(Flipbooks.WalkLeft);
				break;
			case ECatFaceDirection::Right:
				GetSprite()->SetFlipbook(Flipbooks.WalkRight);
				break;
			case ECatFaceDirection::UpLeft:
				GetSprite()->SetFlipbook(Flipbooks.WalkUpLeft);
				break;
			case ECatFaceDirection::UpRight:
				GetSprite()->SetFlipbook(Flipbooks.WalkUpRight);
				break;
			case ECatFaceDirection::DownLeft:
				GetSprite()->SetFlipbook(Flipbooks.WalkDownLeft);
				break;
			case ECatFaceDirection::DownRight:
				GetSprite()->SetFlipbook(Flipbooks.WalkDownRight);
				break;
			default: ;
		}
	}
	else
	{
		switch (CurrentCatFaceDirection) {
		case ECatFaceDirection::Up:
			GetSprite()->SetFlipbook(Flipbooks.IdleUp);
			break;
		case ECatFaceDirection::Down:
			GetSprite()->SetFlipbook(Flipbooks.IdleDown);
			break;
		case ECatFaceDirection::Left:
			GetSprite()->SetFlipbook(Flipbooks.IdleLeft);
			break;
		case ECatFaceDirection::Right:
			GetSprite()->SetFlipbook(Flipbooks.IdleRight);
			break;
		case ECatFaceDirection::UpLeft:
			GetSprite()->SetFlipbook(Flipbooks.IdleUpLeft);
			break;
		case ECatFaceDirection::UpRight:
			GetSprite()->SetFlipbook(Flipbooks.IdleUpRight);
			break;
		case ECatFaceDirection::DownLeft:
			GetSprite()->SetFlipbook(Flipbooks.IdleDownLeft);
			break;
		case ECatFaceDirection::DownRight:
			GetSprite()->SetFlipbook(Flipbooks.IdleDownRight);
			break;
		default: ;
		}
	}
}