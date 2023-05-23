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

void AAICatCharacter::MainCharacterInteractFunction(AMainCatCharacter* MainCatCharacter)
{
	//UE_LOG(LogTemp, Warning, TEXT("Interacted with EnvironmentActor"));
	
	if (!bHasPlayedMiniGame)
	{
		UE_LOG(LogTemp, Warning, TEXT("NPC Mini Game Open!!"));
		
		bHasPlayedMiniGame = true;
		bCanInteract = false;
		if (MainCatCharacter)
		{
			MainCatCharacter->DisableInput(MainCatCharacter->GetLocalViewingPlayerController());
			MainCatCharacter->RemoveInteractableFromSet(this);
		}
		InitializeWidgetAndAddToViewport();
	} else
	{
		UE_LOG(LogTemp, Warning, TEXT("Already Played!!"));
	}
}

void AAICatCharacter::InitializeWidgetAndAddToViewport()
{
	if (!MiniGameWidget && MiniGameWidgetClass)
	{
		MiniGameWidget = CreateWidget<UMiniGameWidget>(GetWorld(), MiniGameWidgetClass);
		UCatSaveGame* CatSaveGame = Cast<UCatSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("MainSlot"), 0));
		int32 Body;
		int32 Texture;
		if (CatSaveGame) {
			Body = CatSaveGame->CatBodyShapeNum;
			Texture = CatSaveGame->CatTextureNum;
		} else {
			Body = 0;
			Texture = 0;
		}
		if (IsValid(MiniGameWidget))
		{
			MiniGameWidget->CatSelection = Body * MaxBodyShapes + Texture;
			MiniGameWidget->AICatSelection = CatBodyShape * MaxBodyShapes + CatTexture;
			MiniGameWidget->AICatAccessory = CatAccessory;
			MiniGameWidget->AddToViewport();
		}
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

	if (InteractionSphere) {
		InteractionSphere->OnComponentBeginOverlap.AddDynamic(this, &AAICatCharacter::OnStartInteract);
		InteractionSphere->OnComponentEndOverlap.AddDynamic(this, &AAICatCharacter::OnEndInteract);
	}
}
