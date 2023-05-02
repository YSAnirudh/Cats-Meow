// Fill out your copyright notice in the Description page of Project Settings.


#include "../Public/MainCatCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"

AMainCatCharacter::AMainCatCharacter()
{
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	bUseControllerRotationYaw = false;
	
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->SetRelativeRotation(FRotator(-20.0f, 0.f, 0.f));
	SpringArmComponent->TargetArmLength = 1500.f;
	//SpringArmComponent->bDoCollisionTest = false;
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
}

void AMainCatCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMainCatCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMainCatCharacter::MoveRight);

	PlayerInputComponent->BindAxis("LookUp", this, &AMainCatCharacter::LookUp);
	PlayerInputComponent->BindAxis("Turn", this, &AMainCatCharacter::Turn);
}

void AMainCatCharacter::SetCurrentAnimationDirection(FVector const& Velocity, TOptional<FMinimalViewInfo> ViewInfo)
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
	// Facing Direction is flipped for main Character
	if (bIsMoving)
	{
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
			CurrentCatFaceDirection = ECatFaceDirection::Right;
		}
		else
		{
			CurrentCatFaceDirection = ECatFaceDirection::Left;
		}
	}
}

