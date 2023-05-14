// Fill out your copyright notice in the Description page of Project Settings.


#include "Environment/BaseEnvironmentActor.h"

#include "MainCatCharacter.h"
#include "Camera/CameraComponent.h"
#include "PaperFlipbookComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABaseEnvironmentActor::ABaseEnvironmentActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	Collision = CreateDefaultSubobject<UBoxComponent>("Collision");
	Collision->SetCollisionProfileName("BlockAllDynamic");
	// Collision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	Collision->SetSimulatePhysics(true);
	Collision->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	SetRootComponent(Collision);
	
	
	Sprite = CreateDefaultSubobject<UPaperFlipbookComponent>("EnvironmentSprite");
	Sprite->SetupAttachment(RootComponent);
	
	// Set Absolute Rotation.
	GetSprite()->SetUsingAbsoluteRotation(true);
}

// Called when the game starts or when spawned
void ABaseEnvironmentActor::BeginPlay()
{
	Super::BeginPlay();
	Collision->AddForce(-GetActorUpVector() * 1000);
}

void ABaseEnvironmentActor::AlignEnvironmentAssetToCamera()
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

	if (!bAlignToCamera)
	{
		TempAdjustYaw = 0.0f;
	}
	
	// Set rotation based on camera movement and player movement
	GetSprite()->SetWorldRotation(FRotator(0.0f, CameraRotation.Yaw + TempAdjustYaw, CameraRotation.Roll));
}

// Called every frame
void ABaseEnvironmentActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bAlignToCamera)
	{
		AlignEnvironmentAssetToCamera();
	}
}

