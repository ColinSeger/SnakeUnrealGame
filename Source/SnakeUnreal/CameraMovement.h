// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Camera/CameraComponent.h"
#include "CameraMovement.generated.h"

UCLASS()
class SNAKEUNREAL_API ACameraMovement : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACameraMovement();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<UCameraComponent> movementCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<AActor*> trackingTargets;

	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintCallable, Category="Control")
	void RotateCamera(float Direction);

	UFUNCTION(BlueprintCallable, Category="Control")
	void EditCameraDistance(float Speed);

	UFUNCTION(BlueprintCallable, Category="Control")
	void CameraTracking();
};
