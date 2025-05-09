// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "GridSystem.generated.h"

UENUM(BlueprintType)
enum class TileEnums : uint8{
	Empty,
	Occupied,
	Apple
};

UCLASS()
class SNAKEUNREAL_API AGridSystem : public AActor
{
	GENERATED_BODY()
	
	//You might wonder why I have multiple arrays instead of 1 array with a struct
	//Basically cache and fun
	//Exporting since it just would not exist otherwise
	UPROPERTY(Export)
	TArray<FVector2D> grid;
	UPROPERTY(Export)
	TArray<TileEnums> tiles;
	UPROPERTY(Export)
	TArray<float> weightFromStart;
	UPROPERTY(Export)
	TArray<float> weightFromEnd;
	UPROPERTY(Export)
	int h = 0;
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
	FVector2D size;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Grid")
	float offset = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Grid")
	TSubclassOf<AActor> wallModel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Grid")
	TSubclassOf<AActor> Apple;

	TArray<FString> mapFiles;

	TArray<AActor*> spawnedActors;

	TArray<int> GetNeighbors(int index);

	FORCEINLINE void BuildGrid();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Grid")
	FVector2D appleLocation = FVector2D();
	
	virtual void OnConstruction(const FTransform& Transform) override;

	UFUNCTION(BlueprintCallable, Category="Grid")
	void CreateGrid(int width, int height);

	UFUNCTION(BlueprintCallable, Category="Grid")
	void DefaultGrid();

	UFUNCTION(BlueprintCallable, Category="Grid")
	void LoadGridFromTxtFile(int level);

	UFUNCTION(BlueprintCallable, Category="Grid")
	FVector2D GetTile(int x, int y);

	UFUNCTION(BlueprintCallable, Category="Grid")
	FVector2D GetRandomEmptyTile();

	UFUNCTION(BlueprintCallable, Category="Grid")
	void ChangeTileStatus(FVector2D location, TileEnums newType);

	UFUNCTION(BlueprintCallable, Category="Grid")
	void SpawnApple();

	UFUNCTION(BlueprintCallable, Category="Grid")
	FVector2D AStarBetweenTiles(FVector2D origin, FVector2D target);

	float GetOffset() const{ return offset;};
};
