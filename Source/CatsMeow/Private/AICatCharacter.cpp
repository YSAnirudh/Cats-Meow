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

void AAICatCharacter::OnMiniGameFinish(bool bCatWon)
{
	bHasPlayedMiniGame = true;
	if (bCatWon)
	{
		AMainCatCharacter* MainCatRef = Cast<AMainCatCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
		if (MainCatRef)
		{
			MainCatRef->EnableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));
			MainCatRef->IncrementHappiness();
		}
	}
	if (MiniGameWidget)
	{
		MiniGameWidget->RemoveFromViewport();
	}
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
		AMainCatCharacter* MainCatRef = Cast<AMainCatCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
		if (MainCatRef)
		{
			MainCatRef->DisableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		}


		UE_LOG(LogTemp, Warning, TEXT("NPC Mini Game Open!!"));
		if (MiniGameWidget)
		{
			MiniGameWidget->AddToViewport();
		}
	} else
	{
		
		UE_LOG(LogTemp, Warning, TEXT("Already Played!!"));
	}
}

void AAICatCharacter::OnStartInteract(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AMainCatCharacter* MainCatRef = Cast<AMainCatCharacter>(OtherActor);

	if (MainCatRef)
	{
		bCanInteract = true;
		MainCatRef->InteractLogicDelegate.AddDynamic(this, &AAICatCharacter::MainCharacterInteractFunction);
	}
}

void AAICatCharacter::OnEndInteract(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AMainCatCharacter* MainCatRef = Cast<AMainCatCharacter>(OtherActor);

	if (MainCatRef)
	{
		bCanInteract = false;
		MainCatRef->InteractLogicDelegate.RemoveDynamic(this, &AAICatCharacter::MainCharacterInteractFunction);
	}
}

void AAICatCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (!MiniGameWidget && MiniGameWidgetClass)
	{
		MiniGameWidget = CreateWidget<UMiniGameWidget>(GetWorld(), MiniGameWidgetClass, TEXT("PettingGameWidget"));
		MiniGameWidget->GameOverDelegate.AddDynamic(this, &AAICatCharacter::OnMiniGameFinish);
		UCatSaveGame* CatSaveGame = Cast<UCatSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("MainSlot"), 0));
		int32 Body = CatSaveGame->CatBodyShapeNum;
		int32 Texture = CatSaveGame->CatTextureNum;
		
		MiniGameWidget->CatSelection = Body * MaxBodyShapes + Texture;
		MiniGameWidget->AICatSelection = CatBodyShape * MaxBodyShapes + CatTexture;
		MiniGameWidget->AICatAccessory = CatAccessory;
	}

	InteractionSphere->OnComponentBeginOverlap.AddDynamic(this, &AAICatCharacter::OnStartInteract);
	InteractionSphere->OnComponentEndOverlap.AddDynamic(this, &AAICatCharacter::OnEndInteract);
	
	PatrolPoint = FVector(1000.f, 1000.f, 0.f);
	const FVector WorldPatrolPoint = UKismetMathLibrary::TransformLocation(
		GetActorTransform(),
		PatrolPoint
	);

	DrawDebugPoint(
		GetWorld(),
		WorldPatrolPoint,
		3.f, FColor::Black, true
	);
	CatAIController = Cast<ACatAIController>(GetController());
	if (!CatAIController)
	{
		CatAIController = Cast<ACatAIController>(GetController());
	}
	if (CatAIController)
	{
		CatAIController->GetCatBlackboardComponent()->SetValueAsVector(TEXT("PatrolPoint"), WorldPatrolPoint);

		//UE_LOG(LogTemp, Warning, TEXT("BBAI: %s"), *CatAIController->GetCatBlackboardComponent()->GetValueAsVector(TEXT("PatrolPoint")).ToString());
		//UE_LOG(LogTemp, Warning, TEXT("World: %s"), *WorldPatrolPoint.ToString());
		
		CatAIController->RunBehaviorTree(BehaviourTree);
	}
}
