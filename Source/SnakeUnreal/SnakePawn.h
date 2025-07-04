// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/World.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Actor.h"
#include "GridSystem.h"
#include "Apple.h"
#include "Kismet/GameplayStatics.h"
#include "Math/MathFwd.h"
#include "Math/UnrealMathUtility.h"
#include "Math/UnrealMathVectorCommon.h"
#include "SnakePawn.generated.h"

UENUM(BlueprintType)
enum class Direction : uint8{
	Forward,
	Back,
	Left,
	Right
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FScoreChanged, int, Score);
UCLASS()
class SNAKEUNREAL_API ASnakePawn : public APawn
{
	GENERATED_BODY()
public:
	// Sets default values for this pawn's properties
	ASnakePawn();
		
	UPROPERTY(BlueprintAssignable, Category= "Snake")
	FScoreChanged scoreChanged;
	struct Tail{
		//TUniqeptr
		FVector newLocation;
		FVector oldLocation;
		FVector2D gridLocation;
		AActor* tail;
	};
protected:
	UPROPERTY(EditAnywhere, BlueprintReadonly, Category="Snake")
	FVector2D currentTile;
	TArray<Tail> tailLocations;
	
	FVector targetLocation;
	FVector currentLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Snake")
	Direction direction = Direction::Forward;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Snake")
	AGridSystem* gridSystem;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Snake")
	int size;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Snake")
	float speed = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Snake")
	TSubclassOf<AActor> tailActor;

	float currentLerp = 0;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;

	UFUNCTION(BlueprintCallable, Category="Snake")
	void TailSizeCheck();

	UFUNCTION(BlueprintCallable, Category="Snake")
	void MovementLogic();

	UFUNCTION(BlueprintCallable, Category="Snake")
	void ResetLerpValue();
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Category="Snake")
	FVector MoveTile();

	UFUNCTION(BlueprintCallable, Category="Snake")
	void SwapDirection(Direction dir);

	UFUNCTION(BlueprintCallable, Category="Snake")
	void AddToTail(int num);

	UFUNCTION(BlueprintCallable, Category="Snake")
	float GetLerpValue()const {return currentLerp;};

	UFUNCTION(BlueprintCallable, Category="Snake")
	float GetSnakeLength()const {return size;};
};
