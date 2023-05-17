// Fill out your copyright notice in the Description page of Project Settings.


#include "../Public/MainCatCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Paper2D/Classes/PaperFlipbookComponent.h"
#include "Saves/CatSaveGame.h"

AMainCatCharacter::AMainCatCharacter()
{
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	bUseControllerRotationYaw = true;
	
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->SetRelativeRotation(FRotator(-20.0f, 0.f, 0.f));
	SpringArmComponent->TargetArmLength = 1500.f;
	SpringArmComponent->bDoCollisionTest = false;
	SpringArmComponent->bInheritPitch = true;
	SpringArmComponent->bInheritRoll = false;
	SpringArmComponent->bInheritYaw = true;
	SpringArmComponent->bEnableCameraLag = true;
	SpringArmComponent->bEnableCameraRotationLag = true;
	SpringArmComponent->CameraLagSpeed = 4.0f;
	SpringArmComponent->CameraRotationLagSpeed = 8.0f;
	SpringArmComponent->CameraLagMaxDistance = 600.f;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(SpringArmComponent);
	CameraComponent->FieldOfView = 55.f;
	CameraComponent->bUsePawnControlRotation = true;
	CameraComponent->PostProcessBlendWeight = 1.0f;
	CameraComponent->PostProcessSettings.bOverride_DepthOfFieldFstop = true;
	CameraComponent->PostProcessSettings.DepthOfFieldFstop = 0.5f;
	CameraComponent->PostProcessSettings.bOverride_DepthOfFieldSensorWidth = true;
	CameraComponent->PostProcessSettings.DepthOfFieldSensorWidth = true;
	CameraComponent->PostProcessSettings.bOverride_DepthOfFieldFocalDistance = true;
	CameraComponent->PostProcessSettings.DepthOfFieldFocalDistance = SpringArmComponent->TargetArmLength;
	CameraComponent->PostProcessSettings.bOverride_DepthOfFieldMinFstop = true;
	CameraComponent->PostProcessSettings.DepthOfFieldMinFstop = 0.0f;
	CameraComponent->PostProcessSettings.MotionBlurAmount = 0.0f;

	// Call Animate function when the player moves.
	OnCharacterMovementUpdated.AddDynamic(this, &AMainCatCharacter::Animate);
	
	TempCatRotation = CameraComponent->GetForwardVector().ToOrientationRotator();
}

// Move Forward/Back
void AMainCatCharacter::MoveForward(float AxisValue)
{
	if (AxisValue != 0.0f)
	{
		const FVector Direction = CameraComponent->GetForwardVector();
		AddMovementInput(Direction, AxisValue);
	}
}

// Move Right/Left
void AMainCatCharacter::MoveRight(float AxisValue)
{
	if (AxisValue != 0.0f)
	{
		const FVector Direction = CameraComponent->GetRightVector();
		AddMovementInput(Direction, AxisValue);
	}
}

// Mouse Movement Up/Down
void AMainCatCharacter::LookUp(float AxisValue)
{
	if (AxisValue != 0)
	{
		AddControllerPitchInput(AxisValue);
	}
}

// Mouse Movement Right/Left
void AMainCatCharacter::Turn(float AxisValue)
{
	if (AxisValue != 0)
	{
		AddControllerYawInput(AxisValue);
	}
}

void AMainCatCharacter::BeginPlay()
{
	Super::BeginPlay();

	LoadPlayerSelectionFromSlot(TEXT("CustomToInit"));
	//SavePlayerSelectionToSlot(TEXT("CustomToInit"));
}

void AMainCatCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	AlignCharacterToCamera();
}

void AMainCatCharacter::OnInteract()
{
	InteractLogicDelegate.Broadcast();
}

void AMainCatCharacter::LoadPlayerSelectionFromSlot(FString SlotName)
{
	if (UCatSaveGame* GameDataInstance = Cast<UCatSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0)))
	{
		CatBodyShape = GameDataInstance->CatBodyShapeNum;
		CatTexture = GameDataInstance->CatTextureNum;
		CatAccessory = GameDataInstance->CatAccessoryNum;
		//UE_LOG(LogTemp, Warning, TEXT("Accessory: %d"), GameDataInstance->CatAccessoryNum);
	}
}

void AMainCatCharacter::SavePlayerSelectionToSlot(FString SlotName)
{
	
}

void AMainCatCharacter::AlignCharacterToCamera()
{
	// Get Camera Rotation to adjust player sprites to camera 
	const FRotator CameraRotation = CameraComponent->GetForwardVector().ToOrientationRotator();

	// Get Sprite rotation
	const FRotator SpriteRotation = GetSprite()->GetForwardVector().ToOrientationRotator();
	
	// A Temp Variable to adjust pitch while moving
	float AdjustYawRotation = -90.0f;
	if (bIsMoving)
	{
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

		TempCatRotation = CameraRotation;
	}
	else
	{
		const FVector Forward = CameraComponent->GetForwardVector();
		const FVector Right = CameraComponent->GetRightVector();

		const float ForwardDot = FMath::Floor(FVector::DotProduct(Forward, CatRight) * 100) / 100;
		const float RightDot = FMath::Floor(FVector::DotProduct(Right, CatRight) * 100) / 100;
		
		if (RightDot > 0.75f)
		{
			// Forward - DO NOT CHANGE
			AdjustYawRotation += 0.0f;
		}
		else if (abs(RightDot) < 0.75 && ForwardDot < 0.0f)
		{
			// Turning camera to the right
			AdjustYawRotation -= 90.0f;
		}
		else if (abs(RightDot) < 0.75 && ForwardDot > 0.0f)
		{
			// Turning camera to the left
			AdjustYawRotation += 90.0f;
		}
		else if (abs(RightDot) > 0.75)
		{
			// Behind
			AdjustYawRotation += 180.0f;
		}
		
		GetSprite()->SetWorldRotation(FRotator(0.0f, TempCatRotation.Yaw + AdjustYawRotation, TempCatRotation.Roll));
		GetAccessorySprite()->SetWorldRotation(FRotator(0.0f, TempCatRotation.Yaw + AdjustYawRotation, TempCatRotation.Roll));
	}
}

// CODE UNSTABLE - DON'T USE
void AMainCatCharacter::ClampCameraPitch() const
{
	const FRotator SpringArmRotation = SpringArmComponent->GetRelativeRotation();
	//UE_LOG(LogTemp, Warning, TEXT("SpringArmRotation.Pitch: %f"), SpringArmRotation.Pitch);
	if (SpringArmRotation.Pitch < -CameraPitchMax)
	{
		
		SpringArmComponent->SetRelativeRotation(
			FRotator(
				-CameraPitchMax,
				SpringArmRotation.Yaw,
				0.0f
			)
		);
	} else if (SpringArmRotation.Pitch > -CameraPitchMin)
	{
		
		SpringArmComponent->SetRelativeRotation(
			FRotator(
				-CameraPitchMin,
				SpringArmRotation.Yaw,
				0.0f
			)
		);
	}
}

void AMainCatCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMainCatCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMainCatCharacter::MoveRight);

	PlayerInputComponent->BindAxis("LookUp", this, &AMainCatCharacter::LookUp);
	PlayerInputComponent->BindAxis("Turn", this, &AMainCatCharacter::Turn);

	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AMainCatCharacter::OnInteract);
}

void AMainCatCharacter::SetCurrentAnimationDirection(FVector const& Velocity, TOptional<FMinimalViewInfo> ViewInfo)
{
	// ViewInfo is set, continue
	if (!ViewInfo.IsSet()) return;
	
	// Calculate forward and Right vector of the character from the ViewInfo
	// Camera Forward and Right Vectors
	const FVector Forward = UKismetMathLibrary::GetForwardVector(ViewInfo.GetValue().Rotation);
	const FVector Right = UKismetMathLibrary::GetRightVector(ViewInfo.GetValue().Rotation);

	// Forward Speed and Right Speed indicate the
	// amount of similarity between Velocity and
	// the Forward and Right Vector (Dot Product)
	// Speed of the cat based on the camera forward and right vectors
	const float ForwardSpeed = FMath::Floor(FVector::DotProduct(Velocity.GetSafeNormal(), Forward) * 100) / 100;
	const float RightSpeed = FMath::Floor(FVector::DotProduct(Velocity.GetSafeNormal(), Right) * 100) / 100;

	// Set is moving by checking the character speed
	bIsMoving = RightSpeed != 0.0f || ForwardSpeed != 0.0f;

	// If the Character is moving, only then update Direction.
	// Facing Direction is flipped for main Character
	if (bIsMoving)
	{
		CatRight = Right;
		if (ForwardSpeed > 0.0f && abs(RightSpeed) < 0.25f)
		{
			CurrentCatFaceDirection = ECatFaceDirection::Up;
		}
		else if (ForwardSpeed > 0.5f && RightSpeed >= 0.25f)
		{
			CurrentCatFaceDirection = ECatFaceDirection::UpLeft;
		}
		else if (ForwardSpeed > 0.5f && RightSpeed <= -0.25f)
		{
			CurrentCatFaceDirection = ECatFaceDirection::UpRight;
		}
		else if (ForwardSpeed < 0.5f && abs(RightSpeed) <= 0.25f)
		{
			CurrentCatFaceDirection = ECatFaceDirection::Down;
		}
		else if (ForwardSpeed < -0.5f && RightSpeed >= 0.25f)
		{
			CurrentCatFaceDirection = ECatFaceDirection::DownLeft;
		}
		else if (ForwardSpeed < -0.5f && RightSpeed <= -0.25f)
		{
			CurrentCatFaceDirection = ECatFaceDirection::DownRight;
		}
		else if (abs(ForwardSpeed) < 0.5f && RightSpeed > 0.0f)
		{
			CurrentCatFaceDirection = ECatFaceDirection::Left;
		}
		else if (abs(ForwardSpeed) < 0.5f && RightSpeed < 0.0f)
		{
			CurrentCatFaceDirection = ECatFaceDirection::Right;
		}
		TempCatFaceDirection = CurrentCatFaceDirection;
	}
	// If the cat isn't moving, update Cat Face Direction according to the camera movement. 
	else {
		const float ForwardDot = FMath::Floor(FVector::DotProduct(Forward, CatRight) * 100) / 100;
		const float RightDot = FMath::Floor(FVector::DotProduct(Right, CatRight) * 100) / 100;
		
		if (TempCatFaceDirection == ECatFaceDirection::Left ||
			TempCatFaceDirection == ECatFaceDirection::UpLeft ||
			TempCatFaceDirection == ECatFaceDirection::DownLeft)
		{
			TempCatFaceDirection = ECatFaceDirection::Left;
		}

		if (TempCatFaceDirection == ECatFaceDirection::Right ||
				TempCatFaceDirection == ECatFaceDirection::UpRight ||
				TempCatFaceDirection == ECatFaceDirection::DownRight)
		{
			TempCatFaceDirection = ECatFaceDirection::Right;
		}
		
		if (RightDot > 0.75f)
		{
			// Forward - DO NOT CHANGE
			switch(TempCatFaceDirection)
			{
			case ECatFaceDirection::Up:
				CurrentCatFaceDirection = ECatFaceDirection::Up;
				break;
			case ECatFaceDirection::Down:
				CurrentCatFaceDirection = ECatFaceDirection::Down;
				break;
			case ECatFaceDirection::Left:
				CurrentCatFaceDirection = ECatFaceDirection::Left;
				break;
			case ECatFaceDirection::Right:
				CurrentCatFaceDirection = ECatFaceDirection::Right;
				break;
			default:;
			}
		}
		else if (abs(RightDot) < 0.75 && ForwardDot < 0.0f)
		{
			// Turning camera to the right
			switch(TempCatFaceDirection)
			{
			case ECatFaceDirection::Up:
				CurrentCatFaceDirection = ECatFaceDirection::Left;
				break;
			case ECatFaceDirection::Down:
				CurrentCatFaceDirection = ECatFaceDirection::Right;
				break;
			case ECatFaceDirection::Left:
				CurrentCatFaceDirection = ECatFaceDirection::Down;
				break;
			case ECatFaceDirection::Right:
				CurrentCatFaceDirection = ECatFaceDirection::Up;
				break;
			default:;
			}
		}
		else if (abs(RightDot) < 0.75 && ForwardDot > 0.0f)
		{
			// Turning camera to the left
			switch(TempCatFaceDirection)
			{
			case ECatFaceDirection::Up:
				CurrentCatFaceDirection = ECatFaceDirection::Right;
				break;
			case ECatFaceDirection::Down:
				CurrentCatFaceDirection = ECatFaceDirection::Left;
				break;
			case ECatFaceDirection::Left:
				CurrentCatFaceDirection = ECatFaceDirection::Up;
				break;
			case ECatFaceDirection::Right:
				CurrentCatFaceDirection = ECatFaceDirection::Down;
				break;
			default:;
			}
		}
		else if (abs(RightDot) > 0.75)
		{
			// Behind
			switch(TempCatFaceDirection)
			{
			case ECatFaceDirection::Up:
				CurrentCatFaceDirection = ECatFaceDirection::Down;
				break;
			case ECatFaceDirection::Down:
				CurrentCatFaceDirection = ECatFaceDirection::Up;
				break;
			case ECatFaceDirection::Left:
				CurrentCatFaceDirection = ECatFaceDirection::Right;
				break;
			case ECatFaceDirection::Right:
				CurrentCatFaceDirection = ECatFaceDirection::Left;
				break;
			default:;
			}
		}
	}
}

void AMainCatCharacter::Animate(float DeltaTime, FVector OldLocation, FVector const OldVelocity)
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

	// Check if Character is moving to play Idle/Walk Animations
	AssignFlipbooks(OldVelocity);
}

