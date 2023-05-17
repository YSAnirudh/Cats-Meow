// Fill out your copyright notice in the Description page of Project Settings.


#include "AICatCharacter.h"

#include "MainCatCharacter.h"
#include "AI/CatAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

AAICatCharacter::AAICatCharacter()
{
	InteractionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Interaction Sphere"));
	InteractionSphere->SetupAttachment(RootComponent);
	InteractionSphere->SetSphereRadius(100.0f);
	InteractionSphere->SetCollisionProfileName(TEXT("Trigger"));
}

void AAICatCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
}

void AAICatCharacter::OnStartInteract(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AMainCatCharacter* MainCatRef = Cast<AMainCatCharacter>(OtherActor);

	if (MainCatRef)
	{
		bCanInteract = true;
	}
}

void AAICatCharacter::OnEndInteract(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AMainCatCharacter* MainCatRef = Cast<AMainCatCharacter>(OtherActor);

	if (MainCatRef)
	{
		bCanInteract = false;
	}
}

void AAICatCharacter::BeginPlay()
{
	Super::BeginPlay();

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

		UE_LOG(LogTemp, Warning, TEXT("BBAI: %s"), *CatAIController->GetCatBlackboardComponent()->GetValueAsVector(TEXT("PatrolPoint")).ToString());
		UE_LOG(LogTemp, Warning, TEXT("World: %s"), *WorldPatrolPoint.ToString());
		
		CatAIController->RunBehaviorTree(BehaviourTree);
	}
}
