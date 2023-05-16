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


	// VARIABLES
protected:
	// FUNCTIONS
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environment", meta = (AllowPrivateAccess = "true"))
	bool bAlignToCamera = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environment", meta = (AllowPrivateAccess = "true"))
	float TempAdjustYaw = -90.0f;
};