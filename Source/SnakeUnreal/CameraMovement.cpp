// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraMovement.h"

// Sets default values
ACameraMovement::ACameraMovement()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACameraMovement::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ACameraMovement::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CameraTracking();
}

// Called to bind functionality to input
void ACameraMovement::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if(movementCamera) return;
	auto owner = GetOwner();
	
	if(owner){
		UActorComponent* component = owner->FindComponentByClass<UCameraComponent>();
		
		movementCamera = Cast<UCameraComponent>(component);
	}
}

void ACameraMovement::RotateCamera(float Direction){
	FRotator rotation = GetActorRotation();

	FRotator toAdd = FRotator(0, Direction, 0);

	rotation += toAdd;

	SetActorRotation(rotation);
}

void ACameraMovement::EditCameraDistance(float Speed){
	if(!movementCamera) return;
	UCameraComponent* localCamera = movementCamera.Get();

	FVector location = localCamera->GetRelativeLocation();

	location += FVector(Speed, 0, 0);

	localCamera->SetRelativeLocation(location);
}

void ACameraMovement::CameraTracking(){
	FVector preLocation = GetActorLocation();
	FVector newLocation = preLocation;
	for (int i = 0; i < trackingTargets.Num(); i++){
		preLocation = newLocation;
		newLocation = trackingTargets[i]->GetActorLocation();
	}
	SetActorLocation(FMath::Lerp(preLocation, newLocation, 0.5f));
}