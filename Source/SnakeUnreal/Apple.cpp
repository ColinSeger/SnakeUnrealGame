// Fill out your copyright notice in the Description page of Project Settings.


#include "Apple.h"
#include "GameFramework/Actor.h"

// Sets default values
AApple::AApple()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));

	// RootComponent = SceneComponent;

	// instancedWalls = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("InstancedWalls"));

	// instancedWalls->SetupAttachment(RootComponent);

	//InstancedFloors = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("InstancedFloors"));

	// InstancedFloors->SetupAttachment(RootComponent);
}
void AApple::OnConstruction(const FTransform& Transform){
	// instancedWalls->ClearInstances();
	// for (AActor*& Actor : SpawnedActors){
	// 	Actor->Destroy();
	// }
	
	// SpawnedActors.Empty();
	// FTransform test = FTransform();
	// instancedWalls->AddInstance(test);
}
// Called when the game starts or when spawned
void AApple::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AApple::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

