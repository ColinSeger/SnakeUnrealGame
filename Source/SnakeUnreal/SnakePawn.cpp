// Fill out your copyright notice in the Description page of Project Settings.


#include "SnakePawn.h"
#include "GameFramework/Actor.h"
#include "Math/MathFwd.h"
#include "Math/UnrealMathUtility.h"


// Sets default values
ASnakePawn::ASnakePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASnakePawn::BeginPlay()
{
	Super::BeginPlay();
	gridSystem = static_cast<AGridSystem*>(UGameplayStatics::GetActorOfClass(GetWorld(), AGridSystem::StaticClass()));

	if(!gridSystem){
		if(GEngine){
			GEngine->AddOnScreenDebugMessage(-1 , 15.f, FColor::Emerald ,FString::Printf(TEXT("Could not find grid")));
		}
		return;
	}

	SetActorLocation(FVector(gridSystem->GetRandomEmptyTile(), 90));

	FVector2D spawn = FVector2D(GetActorLocation() - gridSystem->GetActorLocation());
	//spawn = spawn.RoundToVector();
	
	int32 x = FMath::RoundToInt32(spawn.X);
	int32 y = FMath::RoundToInt32(spawn.Y);
	// FVector2D local = gridSystem->GetTile(spawn.X, spawn.Y);
	float offset = gridSystem->GetOffset();
	currentTile = FVector2D(x / offset, y / offset);
	// I know This is really stupid but for some reason It would not work otherwise
	currentTile.X = (int)currentTile.X;
	currentTile.Y = (int)currentTile.Y;
	// FVector newLocation = FVector(local.X, local.Y, 90);
	// SetActorLocation(newLocation);
	// if(GEngine){
	// 	GEngine->AddOnScreenDebugMessage(-1 , 15.f, FColor::Emerald ,FString::Printf(TEXT("Starting at %d , %d"), (int)currentTile.X, (int)currentTile.Y));
	// }
}
// Called every frame
void ASnakePawn::Tick(float DeltaTime){

	Super::Tick(DeltaTime);
	
	currentLerp += speed * DeltaTime;

	if(currentLerp > 1){
		// ResetLerpValue();
	}
	// MovementLogic();
	// VectorLerp(currentLocation, targetLocation, currentLerp);

	// TailSizeCheck();
}

// Called to bind functionality to input
void ASnakePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent){
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ASnakePawn::SwapDirection(Direction dir){
	this->direction = dir;
}

FVector ASnakePawn::MoveTile(){
	switch (direction) {
		case Direction::Forward:
			currentTile.X +=1;
		break;
		case Direction::Back:
			currentTile.X -=1;
		break;
		case Direction::Left:
			currentTile.Y +=1;
		break;
		case Direction::Right:
			currentTile.Y -=1;
		break;
	}
	//Returns vector of tile location
	return FVector(gridSystem->GetTile(currentTile.X , currentTile.Y), 90);
}

void ASnakePawn::MovementLogic(){
	//Lerp the head of snake
	SetActorLocation(FMath::Lerp(currentLocation, targetLocation, currentLerp));

	//Loop over tails and lerp them
	for (Tail& tail : tailLocations) {
		tail.tail->SetActorLocation(FMath::Lerp(tail.oldLocation, tail.newLocation, currentLerp));
	}
}

void ASnakePawn::ResetLerpValue(){
	if(tailLocations.Num() > 0){
		for(int i = 0; i< tailLocations.Num(); i++){
			gridSystem->ChangeTileStatus(tailLocations[i].gridLocation, TileEnums::Empty);
		}
		for(int i = tailLocations.Num() -1; i >= 1; i--){
			//This looks bad
			FVector2D gridDirection = (tailLocations[i-1].gridLocation - tailLocations[i].gridLocation);
			if(gridDirection.X != 0 || gridDirection.Y != 0){
				tailLocations[i].gridLocation += gridDirection;
				gridSystem->ChangeTileStatus(tailLocations[i].gridLocation, TileEnums::Occupied);
			}
			tailLocations[i].oldLocation = tailLocations[i].newLocation;
			tailLocations[i].newLocation = tailLocations[i-1].newLocation;
		}
		tailLocations[0].gridLocation = currentTile;
		tailLocations[0].oldLocation = tailLocations[0].newLocation;
		tailLocations[0].newLocation = targetLocation;
	}
	currentLerp = 0;
	currentLocation = targetLocation;
	targetLocation = MoveTile();
}

void ASnakePawn::AddToTail(int num){
	size += num;
	scoreChanged.Broadcast(size);
}

void ASnakePawn::TailSizeCheck(){
	if (tailLocations.Num() >= size) {
		return;
	}
	FActorSpawnParameters SpawnInfo;

	FVector tailLocation = currentLocation;
	FVector2D gridLocation;
	if(tailLocations.Num()> 0){
		gridLocation = tailLocations.Last().gridLocation;
	}else{
		gridLocation = currentTile;
	}
	if(tailLocations.Num() >= 1){
		tailLocation = tailLocations.Last().oldLocation;
	}

	AActor* spawnedTail = GetWorld()->SpawnActor<AActor>(tailActor, tailLocation, GetActorRotation(), SpawnInfo);
	Tail tail{
		tailLocation,
		spawnedTail->GetActorLocation(),
		gridLocation,
		spawnedTail
	};
	
	tailLocations.Add(tail);
}

void ASnakePawn::BeginDestroy(){
	Super::BeginDestroy();
	for(int i = 0; i < tailLocations.Num(); i++){
		tailLocations[i].tail->Destroy();
	}
}