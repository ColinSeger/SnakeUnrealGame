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

}

// Called to bind functionality to input
void ACameraMovement::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//PlayerInputComponent.
}

void ACameraMovement::RotateCamera(){
	
}

void ACameraMovement::EditCameraDistance(){

}
