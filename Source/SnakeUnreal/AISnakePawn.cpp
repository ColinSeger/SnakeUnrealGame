// Fill out your copyright notice in the Description page of Project Settings.


#include "AISnakePawn.h"

void AAISnakePawn::Tick(float DeltaTime){
    Super::Tick(DeltaTime);
    if(currentLerp > 1){
        FVector2D test = gridSystem->AStarBetweenTiles(currentTile, gridSystem->appleLocation);    
        FVector2D bro = gridSystem->GetTile(currentTile.X, currentTile.Y); 
        test -= bro;
        test /= gridSystem->GetOffset();
        if(test.X > 0.3){
            direction = Direction::Forward;
        }else if(test.X < -0.3){
            direction = Direction::Back;
        }else if(test.Y > 0.3){
            direction = Direction::Left;
        }else if (test.Y < -0.3){
            direction = Direction::Right;
        }
        if(GEngine){
            GEngine->AddOnScreenDebugMessage(-1 , 15.f, FColor::Emerald ,FString::Printf(TEXT("Starting at %d , %d"), (int)test.X, (int)test.Y));
        }

        ResetLerpValue();
    }
}