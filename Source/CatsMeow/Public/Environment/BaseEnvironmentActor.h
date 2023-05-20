// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MainCatCharacter.h"
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
	UFUNCTION()
	FORCEINLINE int32 GetCanInteract() const { return bCanInteract; }
	UFUNCTION()
	virtual void SetCanInteract(bool bInteract);
	
	UFUNCTION()
	virtual void MainCharacterInteractFunction();
	
	// VARIABLES
protected:
	// FUNCTIONS
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction", meta = (AllowPrivateAccess = "true"))
	bool bHasPlayedMiniGame = false;

	class UMiniGameWidget* MiniGameWidget = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MiniGame", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UMiniGameWidget> MiniGameWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction", meta = (AllowPrivateAccess = "true"))
	bool bIsInteractable = false;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction", meta = (AllowPrivateAccess = "true"))
	bool bCanInteract = false;
	// VARIABLES
private:
	// FUNCTIONS
	void AlignEnvironmentAssetToCamera();

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
};
