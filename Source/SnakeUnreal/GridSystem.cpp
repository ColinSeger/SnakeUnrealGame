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
	weight.Empty();
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
	weight.Reserve(reserveSize);
	for(int x = 0; x < width; x++){
		for(int y = 0; y < height; y++){
			FVector2D cord = FVector2D(x * offset, y * offset);
			cord += FVector2D(GetActorLocation());
			grid.Add(cord);
			weight.Add(0);
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
		int number = FMath::RandRange(0, tiles.Num());
		if(tiles[number] == TileEnums::Empty){
			return  grid[number];
		}	
	}
}

FVector2D AGridSystem::AStarBetweenTiles(FVector2D origin, FVector2D target){
	FVector2D result = FVector2D();
	TArray<int> toBeSearched;
	TArray<int> checkedTiles;
	int startIndex = origin.Y + h * origin.X;
	int targetIndex = target.Y + h * target.X;
	toBeSearched.Add(startIndex);
	weight[startIndex] = 0;

	TArray<int> indexPath;
	
	while (!toBeSearched.IsEmpty()) {
		int currentTileIndex = toBeSearched[0];
		for (int index : toBeSearched) {
			//Makes sure no other tile has a lower value
			if(weight[currentTileIndex] > toBeSearched.IndexOfByKey(index)){
				currentTileIndex = toBeSearched.IndexOfByKey(index);
			}
		}

		checkedTiles.Add(currentTileIndex);
		toBeSearched.Remove(currentTileIndex);
		if(currentTileIndex == targetIndex){
			result = grid[indexPath[0]];//Change this to actually be next tile to go to
			return result;
		}
		// FVector2D currentTile = grid[currentTileIndex];
		
		//Checks neighbors on X axis I think
		if(!checkedTiles.Contains(currentTileIndex -1)){
			if (currentTileIndex > 0) {
				float costToTile = weight[currentTileIndex] + 1;
			}
		}
		if(!checkedTiles.Contains(currentTileIndex +1)){
			if (currentTileIndex < checkedTiles.Num()) {
				float costToTile = weight[currentTileIndex] + 1;
			}
		}

		checkedTiles.Add(currentTileIndex);
	}

	return result;
}

void AGridSystem::SpawnApple(){
	if(!Apple) return;
	FVector2D location = GetRandomEmptyTile();
	
	AActor* spawnedApple = GetWorld()->SpawnActor<AActor>(Apple, FVector(location, 0), GetActorRotation());
	spawnedActors.Add(spawnedApple);
}