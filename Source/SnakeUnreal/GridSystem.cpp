// Fill out your copyright notice in the Description page of Project Settings.


#include "GridSystem.h"
#include "Components/BoxComponent.h"
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
}

// Called when the game starts or when spawned
void AGridSystem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGridSystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGridSystem::CreateGrid(int width, int height){
 	// UE_LOG(LogTemp, Warning, TEXT("Created Grid"))
	if(GEngine){
		GEngine->AddOnScreenDebugMessage(-1 , 15.f, FColor::Emerald ,TEXT("Grid Created"));		
	}
	grid.Empty();
	tiles.Empty();
	weight.Empty();
	w = height;
	// Size.Y = height;
	// Size.X = height;
	int reserveSize = width * height;
	grid.Reserve(reserveSize);
	tiles.Reserve(reserveSize);
	weight.Reserve(reserveSize);
	for(int x = 0; x < width; x++){
		for(int y = 0; y < height; y++){
			FVector2D cord = FVector2D(x * offset, y * offset);
			grid.Add(cord);
			weight.Add(0);
			if(y == 0 || x == 0){
				tiles.Add(TileEnums::Occupied);		
			}else{
				tiles.Add(TileEnums::Empty);				
			}
		}
	}
}

FVector2D AGridSystem::GetTile(int x, int y){
	int index = y + w * x;
	if(index >= grid.Num() || index < 0) return FVector2D();
	return grid[index];
	
	// if(GEngine){
	// 	GEngine->AddOnScreenDebugMessage(-1 , 15.f, FColor::Emerald ,FString::Printf(TEXT("You got tile X %d Y %d"), result.X, result.Y));
	// 	GEngine->AddOnScreenDebugMessage(-1 , 15.f, FColor::Emerald ,FString::Printf(TEXT("You got tile type %d"), tiles[index]));
	// }
}

void AGridSystem::MoveActor(AActor* actor){
	FVector location = actor->GetActorTransform().GetLocation();
	FVector add = FVector(0,1,0);
	actor->SetActorLocation(location += add);
	if(GEngine){
		GEngine->AddOnScreenDebugMessage(-1 , 15.f, FColor::Emerald ,FString::Printf(TEXT("You got test")));
	}
}

FVector2d AGridSystem::NewLocation(FVector2D oldLocation){
	int index = oldLocation.Y + w * oldLocation.X;
	if(index >= grid.Num() || index < 0) return FVector2D();

	FVector2D result = grid[index];
	return FVector2D(result.X , result.Y);
}

FVector2D AGridSystem::GetRandomEmptyTile(){
	while (true) {
		int number = FMath::RandRange(0, tiles.Num());

		if(tiles[number] == TileEnums::Empty){
			return  grid[number] / offset;
		}	
	}
}