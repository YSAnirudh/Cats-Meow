// Fill out your copyright notice in the Description page of Project Settings.


#include "CatCharacter.h"

#include "MainCatCharacter.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Paper2D/Classes/PaperFlipbookComponent.h"


ACatCharacter::ACatCharacter()
{
	AccessorySprite = CreateDefaultSubobject<UPaperFlipbookComponent>("AccessorySprite");
	AccessorySprite->SetupAttachment(GetSprite());
	
	// Set Absolute Rotation and Initialize the default Flipbook.
	GetSprite()->SetUsingAbsoluteRotation(true);
	
	bIsMoving = false;

	InitializeFlipbooks();
}

void ACatCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	// Call Animate function Everytick.
	Animate(DeltaSeconds, GetActorLocation(), GetVelocity());
	
	AlignCharacterToCamera();
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
		GetSprite()->SetFlipbook(CatAnimationFlipbooks[CatTexture * MaxBodyShapes + CatBodyShape].IdleUp);
	}
	if (CheckAccessoryNumber(CatAccessory))
	{
		GetAccessorySprite()->SetFlipbook(CatAccessoryFlipbooks[CatAccessory].IdleUp);
	}
}

void ACatCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	LoadFlipbooks();
}

void ACatCharacter::SetCurrentAnimationDirection(FVector const& Velocity, TOptional<FMinimalViewInfo> ViewInfo)
{
	// Calculate forward and Right vector of the character from the PlayerCameraPosition
	const FVector Forward = GetSprite()->GetRightVector();
	const FVector Right = GetSprite()->GetForwardVector();

	// Forward Speed and Right Speed indicate the
	// amount of similarity between Velocity and
	// the Forward and Right Vector (Dot Product)
	const float ForwardSpeed = FMath::Floor(FVector::DotProduct(Velocity.GetSafeNormal(), Forward) * 100) / 100;
	const float RightSpeed = FMath::Floor(FVector::DotProduct(Velocity.GetSafeNormal(), Right) * 100) / 100;

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
	// Placeholder ViewInfo for AI cats (No Camera)
	const TOptional<FMinimalViewInfo> ViewInfo;
	
	// Set Direction Based on Velocity
	SetCurrentAnimationDirection(OldVelocity, ViewInfo);

	// Assign CatAnimationFlipbooks based on direction
	AssignFlipbooks(OldVelocity);
}

void ACatCharacter::AlignCharacterToCamera()
{
	// Get Camera Rotation to adjust player sprites to camera
	const AMainCatCharacter* MainCatCharacter = Cast<AMainCatCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (!MainCatCharacter)
	{
		return;
	}
	const UCameraComponent* PlayerCamera = MainCatCharacter->GetPlayerCameraComponent();
	if (!PlayerCamera)
	{
		return;
	}
	const FRotator CameraRotation = (GetActorLocation() - PlayerCamera->GetComponentLocation()).GetSafeNormal().ToOrientationRotator();

	// A Temp Variable to adjust pitch while moving
	float AdjustYawRotation = -90.0f;
	if (CurrentCatFaceDirection == ECatFaceDirection::UpLeft ||
			CurrentCatFaceDirection == ECatFaceDirection::DownRight)
	{
		AdjustYawRotation -= SpriteTiltYaw;
	}
	else if (CurrentCatFaceDirection == ECatFaceDirection::DownLeft ||
		CurrentCatFaceDirection == ECatFaceDirection::UpRight)
	{
		AdjustYawRotation += SpriteTiltYaw;
	}
	
	// Set rotation based on camera movement and player movement
	GetSprite()->SetWorldRotation(FRotator(0.0f, CameraRotation.Yaw + AdjustYawRotation, CameraRotation.Roll));
	GetAccessorySprite()->SetWorldRotation(FRotator(0.0f, CameraRotation.Yaw + AdjustYawRotation, CameraRotation.Roll));
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

void ACatCharacter::LoadFlipbooks()
{
	for (int i = 0; i < MaxBodyShapes; i++)
	{
		for (int j = 0; j < MaxTextures; j++)
		{
			LoadCatAnimationFlipbooks(i, j);
		}
	}

	for (int i = 0; i < MaxAccessories; i++)
	{
		LoadCatAccessoryFlipbooks(i);
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
		return false;
	}
	return true;
}

bool ACatCharacter::CheckAccessoryNumber(int32 AccessoryNo) const
{
	if (AccessoryNo < 0 || AccessoryNo >= MaxAccessories)
	{
		return false;
	}
	return true;
}