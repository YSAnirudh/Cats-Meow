// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseEnvironmentActor.generated.h"

UCLASS()
class CATSMEOW_API ABaseEnvironmentActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// FUNCTIONS
	// Sets default values for this actor's properties
	ABaseEnvironmentActor();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** Returns Accessory Sprite SubObject **/
	FORCEINLINE class UPaperFlipbookComponent* GetSprite() const { return Sprite; }

	// Delegate Functions
	UFUNCTION()
	void OnStartInteract(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
	void OnEndInteract(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	// VARIABLES
protected:
	// FUNCTIONS
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// VARIABLES
private:
	// FUNCTIONS
	void AlignEnvironmentAssetToCamera();

	UFUNCTION()
	virtual void MainCharacterInteractFunction();

	// VARIABLES
	// Accessory Sprite
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Environment", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UPaperFlipbookComponent> Sprite;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Environment", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UBoxComponent> Collision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Environment", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USphereComponent> InteractionSphere;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environment", meta = (AllowPrivateAccess = "true"))
	bool bAlignToCamera = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environment", meta = (AllowPrivateAccess = "true"))
	float TempAdjustYaw = -90.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction", meta = (AllowPrivateAccess = "true"))
	bool bIsInteractable = false;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction", meta = (AllowPrivateAccess = "true"))
	bool bCanInteract = false;
};
