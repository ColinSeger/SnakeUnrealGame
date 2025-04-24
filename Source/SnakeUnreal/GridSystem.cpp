// Fill out your copyright notice in the Description page of Project Settings.


#include "GridSystem.h"
#include "Components/BoxComponent.h"
#include "Containers/Array.h"
#include "Containers/UnrealString.h"
#include "CoreGlobals.h"
#include "Engine/Engine.h"
#include "Engine/EngineTypes.h"
#include "GameFramework/Actor.h"
#include "HAL/Platform.h"
#include "Logging/LogMacros.h"
#include "Logging/LogVerbosity.h"
#include "Math/Color.h"
#include "Math/MathFwd.h"
// #include "Components/BoxComponent.h"
#include "Math/UnrealMathUtility.h"
#include "MathUtil.h"
#include <cmath>

// Sets default values
AGridSystem::AGridSystem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	// PrimaryActorTick.bCanEverTick = true;

	// Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Tile!"));
	// grid.Empty();
	// tiles.Empty();
	// weight.Empty();
	// // Box->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	// int reserveSize = Size.X * Size.Y;
	// grid.Reserve(reserveSize);
	// tiles.Reserve(reserveSize);
	// weight.Reserve(reserveSize);
	// for(int x = 0; x < Size.X; x++){
	// 	for(int y = 0; y < Size.Y; y++){
	// 		FVector2D cord = FVector2D(x * offset, y * offset);
	// 		grid.Add(cord);
	// 		weight.Add(0);
	// 		if(y == 0 || x == 0){
	// 			tiles.Add(TileEnums::Occupied);
	// 		}else{
	// 			tiles.Add(TileEnums::Empty);				
	// 		}
	// 	}
	// }
	// if(GEngine){
	// 	GEngine->AddOnScreenDebugMessage(-1 , 15.f, FColor::Emerald ,TEXT("Grid Created"));		
	// }
	// CreateGrid(size.X, size.Y);
}
void AGridSystem::OnConstruction(const FTransform& Transform){
	grid.Empty();
	tiles.Empty();
	weightFromStart.Empty();
	for (AActor*& actor : spawnedActors) {
		actor->Destroy();
	}
	spawnedActors.Empty();
	CreateGrid(size.X, size.Y);
	//SpawnApple();
}
// Called when the game starts or when spawned
void AGridSystem::BeginPlay()
{
	Super::BeginPlay();
	
	if(GEngine){
		GEngine->AddOnScreenDebugMessage(-1 , 15.f, FColor::Emerald ,TEXT("Grid Created"));
		GEngine->AddOnScreenDebugMessage(-1 , 15.f, FColor::Emerald ,FString::Printf(TEXT("Tile amount %d"), grid.Num()));
	}
}

void AGridSystem::CreateGrid(int width, int height){
 	// UE_LOG(LogTemp, Warning, TEXT("Created Grid"))
	
	h = height;
	// Size.Y = height;
	// Size.X = height;
	uint32 reserveSize = width * height;
	grid.Reserve(reserveSize);
	tiles.Reserve(reserveSize);
	weightFromStart.Reserve(reserveSize);
	for(int x = 0; x < width; x++){
		for(int y = 0; y < height; y++){
			FVector2D cord = FVector2D(x * offset, y * offset);
			cord += FVector2D(GetActorLocation());
			grid.Add(cord);
			weightFromStart.Add(reserveSize);
			weightFromEnd.Add(reserveSize);
			TileEnums tileStatus = TileEnums::Empty;
			if(y == 0 || x == 0 || x == width-1 || y == height-1){
				//tiles.Add(TileEnums::Occupied);
				tileStatus = TileEnums::Occupied;
			}
			tiles.Add(tileStatus);
		}
	}
	for (int i = 0; i < tiles.Num(); i++) {
		if(!wallModel || tiles[i] == TileEnums::Empty) continue;
		FVector spawnLocation = FVector(grid[i], GetActorLocation().Z);
		AActor* wall = GetWorld()->SpawnActor<AActor>(wallModel, spawnLocation, GetActorRotation());
		spawnedActors.Add(wall);
		wall->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
	}
	//SpawnApple();
	if(GEngine){
		GEngine->AddOnScreenDebugMessage(-1 , 15.f, FColor::Emerald ,TEXT("Grid Created"));
		GEngine->AddOnScreenDebugMessage(-1 , 15.f, FColor::Emerald ,FString::Printf(TEXT("Tile amount %d"), grid.Num()));
	}
}

FVector2D AGridSystem::GetTile(int x, int y){
	int index = y + h * x;
	if(index >= grid.Num() || index < 0) return FVector2D();
	return grid[index];
}

FVector2D AGridSystem::GetRandomEmptyTile(){
	while (true) {
		int number = FMath::RandRange(0, tiles.Num() -1);
		if(tiles[number] == TileEnums::Empty){
			return  grid[number];
		}	
	}
}

float GetDistance(FVector2D origin, FVector2D target){
	int distX = FMath::Abs(origin.X - target.X);
	int distY = FMath::Abs(origin.Y - target.Y);
	if(distX > distY){
		return 10* distY + (distX - distY);
	}else{
		return 10* distX + (distY - distX);
	}
}
int GetStartingTile(TArray<FIntVector2> indexPath){
	int selectedIndex = indexPath.Num()-1;
	while (true)
	{
		if(indexPath[selectedIndex].X < 1){
			return indexPath[selectedIndex].Y;
		}else{
			selectedIndex = indexPath[selectedIndex].X;
		}
	}
}

FVector2D AGridSystem::AStarBetweenTiles(FVector2D origin, FVector2D target){
	FVector2D result = FVector2D();
	for(float weight : weightFromStart){
		weight = INFINITY;
	}
	for(float weight : weightFromEnd){
		weight = INFINITY;
	}
	TArray<int> toBeSearched;
	TArray<int> checkedTiles;

	int startIndex = origin.Y + h * origin.X;
	int targetIndex = target.Y + h * target.X;
	if(startIndex == targetIndex){
		return FVector2D();
	}

	toBeSearched.Add(startIndex);
	weightFromStart[startIndex] = 0;
	weightFromEnd[startIndex] = GetDistance(grid[startIndex], grid[targetIndex]);

	TArray<FIntVector2> indexPath;
	indexPath.Add(FIntVector2(-1, startIndex));

	while (!toBeSearched.IsEmpty()) {
		int currentTileIndex = toBeSearched[0];
		int indexTo = 0;
		for (int index = 0; index < toBeSearched.Num(); index++) {
			float currentFCost = weightFromStart[currentTileIndex] + weightFromEnd[currentTileIndex];
			float searchFCost = weightFromStart[toBeSearched[index]] + weightFromEnd[toBeSearched[index]];
			//Makes sure no other tile has a lower value
			if(currentFCost > searchFCost || (currentFCost == searchFCost && weightFromEnd[currentTileIndex] > weightFromEnd[toBeSearched[index]])){
				currentTileIndex = toBeSearched[index];
				for(int i = 0; i< indexPath.Num(); i++){
					if(indexPath[i].Y == toBeSearched[index]){
						indexTo = i;
					}
				}
			}
		}

		checkedTiles.Add(currentTileIndex);
		toBeSearched.Remove(currentTileIndex);
		if(currentTileIndex == targetIndex){
			result = grid[GetStartingTile(indexPath)];//Change this to actually be next tile to go to
			// if(GEngine){
			// 	GEngine->AddOnScreenDebugMessage(-1 , 1.f, FColor::Emerald ,FString::Printf(TEXT("Tile %d %d"), result.X, result.Y));
			// }
			return FVector2D(result.X,result.Y);
		}
		// FVector2D currentTile = grid[currentTileIndex];
		TArray<int> neighbors;
		neighbors.Reserve(4);

		//Assigns tile index of up down left right
		neighbors.Add(currentTileIndex-1);
		neighbors.Add(currentTileIndex+1);
		neighbors.Add(currentTileIndex+h);
		neighbors.Add(currentTileIndex-h);
		//Checks neighbors on Y axis I think
		for(int i = 0; i < neighbors.Num(); i++){
			//If out of bounds or has already been checked it moves on
			if(neighbors[i] >= grid.Num() || 0 > neighbors[i] || checkedTiles.Contains(neighbors[i])) continue;

			float currentWeight = weightFromStart[currentTileIndex] + 1;
			float endWeight = GetDistance(grid[neighbors[i]], grid[targetIndex]);
			weightFromEnd[neighbors[i]] = endWeight;

			if(currentWeight < weightFromStart[neighbors[i]] || !toBeSearched.Contains(neighbors[i])){
				//float Xdistance = FMath::Abs()
				weightFromStart[neighbors[i]] = currentWeight;
				if(!toBeSearched.Contains(neighbors[i])){
					toBeSearched.Add(neighbors[i]);
				}
				indexPath.Add(FIntVector2(indexTo , neighbors[i]));
			}
		}
		neighbors.Empty();
		checkedTiles.Add(currentTileIndex);
		// indexTo = indexPath.Num()-1;
	}

	return result;
}

void AGridSystem::SpawnApple(){
	if(!Apple) return;
	FVector2D location = GetRandomEmptyTile();

	appleLocation = location - FVector2D(GetActorLocation());
	appleLocation /= offset;
	
	AActor* spawnedApple = GetWorld()->SpawnActor<AActor>(Apple, FVector(location, 0), GetActorRotation());
	spawnedActors.Add(spawnedApple);
}