// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "GridSystem.generated.h"

UCLASS()
class SNAKEUNREAL_API AGridSystem : public AActor
{
	GENERATED_BODY()
	enum class TileEnums{
		Empty,
		Occupied,
		Apple
	};
	//You might wonder why I have multiple arrays instead of 1 array with a struct
	//Basically cache and fun
	TArray<FVector2d> grid;
	TArray<float> weight;
	TArray<TileEnums> tiles;
	int w = 0;
public:	
	// Sets default values for this actor's properties
	AGridSystem();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Grid")
	// UBoxComponent* Box;	
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Grid")
	// FVector test;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Grid")
	float offset = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Grid")
	TSubclassOf<AActor> wallModel;

	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Grid")
	// FVector2D Size = FVector2D(10, 10);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category="Grid")
	void CreateGrid(int width, int height);

	UFUNCTION(BlueprintCallable, Category="Grid")
	FVector2D GetTile(int x, int y);

	UFUNCTION(BlueprintCallable, Category="Grid")
	void MoveActor(UPARAM(ref) AActor* actor);

	UFUNCTION(BlueprintCallable, Category="Grid")
	FVector2D NewLocation(FVector2D oldLocation);

	UFUNCTION(BlueprintCallable, Category="Grid")
	FVector2D GetRandomEmptyTile();
};
