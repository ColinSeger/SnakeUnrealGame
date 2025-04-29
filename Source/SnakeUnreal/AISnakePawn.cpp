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
            //This is a bootleg solution to a stupid a*
            if(direction != Direction::Back){
                direction = Direction::Forward;
            }else{
                direction = Direction::Left;
            }
            
        }else if(test.X < -0.3){
            if(direction != Direction::Forward){
                direction = Direction::Back;
            }else{
                direction = Direction::Left;
            }

            
        }else if(test.Y > 0.3){
            if(direction != Direction::Right){
                direction = Direction::Left;
            }else{
                direction = Direction::Forward;
            }

        }else if (test.Y < -0.3){
            if(direction != Direction::Left){
                direction = Direction::Right;
            }else{
                direction = Direction::Forward;
            }
        }
        if(GEngine){
            GEngine->AddOnScreenDebugMessage(-1 , 15.f, FColor::Emerald ,FString::Printf(TEXT("Starting at %d , %d"), (int)test.X, (int)test.Y));
        }

        ResetLerpValue();
    }
}