// Fill out your copyright notice in the Description page of Project Settings.


#include "SnakePawn.h"


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

	if(!gridSystem) return;

	// FVector2D spawn = gridSystem->GetRandomEmptyTile();
	// FVector2D local = gridSystem->GetTile(spawn.X, spawn.Y);
	// currentTile = spawn;
	// FVector newLocation = FVector(local.X, local.Y, 90);
	// SetActorLocation(newLocation);
}
// Called every frame
void ASnakePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	currentLerp += speed * DeltaTime;

	if(currentLerp > 1){
		currentLerp = 0;
		currentLocation = targetLocation;
		targetLocation = MoveTile();
	}
	SetActorLocation(FMath::Lerp(currentLocation, targetLocation, currentLerp));
	// VectorLerp(currentLocation, targetLocation, currentLerp);
	if(tailLocations.Num() <= 1){
		
	}
	if (tailLocations.Num() < size) {
		FActorSpawnParameters SpawnInfo;
		//asd
		// ASnakePawn* test = GetWorld()->SpawnActor<ASnakePawn>(ASnakePawn::StaticClass(), GetActorLocation(), GetActorRotation(), SpawnInfo);
		AApple* test = GetWorld()->SpawnActor<AApple>(AApple::StaticClass(), GetActorLocation(), GetActorRotation(), SpawnInfo);
		Tail tail{
			GetActorLocation(),
			test->GetActorLocation(),
			currentTile,
			test
		};
		tailLocations.Add(tail);
		// tailLocations.Add(Tail(
		// 	test,

		// ));
	}
	for(int i = 0; i < tailLocations.Num(); i++){
		//This looks bad
		tailLocations[i].tail->SetActorLocation(FMath::Lerp(tailLocations[i].oldLocation, tailLocations[i].newLocation, currentLerp));
	}
}

// Called to bind functionality to input
void ASnakePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
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

	FVector2D tileLocation = gridSystem->GetTile(currentTile.X , currentTile.Y);

	FVector newLocation = FVector(tileLocation.X, tileLocation.Y, 90);
	return newLocation;
	//SetActorLocation(newLocation);
}

void ASnakePawn::MoveSnake(){
	FVector2D tileLocation = gridSystem->GetTile(currentTile.X , currentTile.Y);

	FVector newLocation = FVector(tileLocation.X, tileLocation.Y, GetActorLocation().Z);

	SetActorLocation(newLocation);
}

void ASnakePawn::MoveTail(){
	// while (tailLocations.Num() < size) {
	// 	Tail newTail = Tail{
	// 		gridSystem,
	// 		currentTile
	// 	};
	// 	tailLocations.Add(newTail);
	// }
	// for(int i = 0; i < size; i++){
	// 	tailLocations[i];
	// }
}