// Fill out your copyright notice in the Description page of Project Settings.


#include "AICatCharacter.h"

#include "MainCatCharacter.h"
#include "AI/CatAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Saves/CatSaveGame.h"
#include "UI/MiniGameWidget.h"

AAICatCharacter::AAICatCharacter()
{
	InteractionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Interaction Sphere"));
	InteractionSphere->SetupAttachment(RootComponent);
	InteractionSphere->SetSphereRadius(100.0f);
	InteractionSphere->SetCollisionProfileName(TEXT("Trigger"));

	CatBodyShape = FMath::RandRange(0, MaxBodyShapes-1);
	CatTexture = FMath::RandRange(0, MaxTextures-1);
	CatAccessory = FMath::RandRange(0, MaxAccessories-1);
}

void AAICatCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
}

void AAICatCharacter::MainCharacterInteractFunction()
{
	//UE_LOG(LogTemp, Warning, TEXT("Interacted with EnvironmentActor"));
	
	if (!bHasPlayedMiniGame)
	{
		UE_LOG(LogTemp, Warning, TEXT("NPC Mini Game Open!!"));

		if (!IsValid(MiniGameWidget))
		{
			UE_LOG(LogTemp, Warning, TEXT("No Widget"));
			return;
		}
		
		if (IsValid(MiniGameWidget) && !MiniGameWidget->IsInViewport())
		{
			MiniGameWidget->AddToViewport();
			bHasPlayedMiniGame = true;
			AMainCatCharacter* MainCatRef = Cast<AMainCatCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
			if (MainCatRef)
			{
				bCanInteract = false;
				MainCatRef->DisableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));
				MainCatRef->RemoveInteractableFromSet(this);
			}
		}
	} else
	{
		UE_LOG(LogTemp, Warning, TEXT("Already Played!!"));
	}
}

void AAICatCharacter::OnStartInteract(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bHasPlayedMiniGame)
	{
		return;
	}
	AMainCatCharacter* MainCatRef = Cast<AMainCatCharacter>(OtherActor);

	if (MainCatRef)
	{
		MainCatRef->AddInteractableToSet(this);
	}
}

void AAICatCharacter::OnEndInteract(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (bHasPlayedMiniGame)
	{
		return;
	}
	AMainCatCharacter* MainCatRef = Cast<AMainCatCharacter>(OtherActor);

	if (MainCatRef)
	{
		MainCatRef->RemoveInteractableFromSet(this);
	}
}

void AAICatCharacter::BeginPlay()
{
	Super::BeginPlay();

	bHasPlayedMiniGame = false;

	if (!MiniGameWidget && MiniGameWidgetClass)
	{
		MiniGameWidget = CreateWidget<UMiniGameWidget>(GetWorld(), MiniGameWidgetClass, TEXT("PettingGameWidget"));
		UCatSaveGame* CatSaveGame = Cast<UCatSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("MainSlot"), 0));
		if (CatSaveGame)
		{
			int32 Body = CatSaveGame->CatBodyShapeNum;
			int32 Texture = CatSaveGame->CatTextureNum;

			if (IsValid(MiniGameWidget))
			{
				MiniGameWidget->CatSelection = Body * MaxBodyShapes + Texture;
				MiniGameWidget->AICatSelection = CatBodyShape * MaxBodyShapes + CatTexture;
				MiniGameWidget->AICatAccessory = CatAccessory;
			}
		}
	}

	InteractionSphere->OnComponentBeginOverlap.AddDynamic(this, &AAICatCharacter::OnStartInteract);
	InteractionSphere->OnComponentEndOverlap.AddDynamic(this, &AAICatCharacter::OnEndInteract);
	
	PatrolPoint = FVector(-610.f, 0.f, 0.f);
	const FVector WorldPatrolPoint = UKismetMathLibrary::TransformLocation(
		GetActorTransform(),
		PatrolPoint
	);

	DrawDebugPoint(
		GetWorld(),
		WorldPatrolPoint,
		3.f, FColor::Black, true
	);
}
