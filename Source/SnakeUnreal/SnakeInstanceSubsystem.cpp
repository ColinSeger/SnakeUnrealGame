// Fill out your copyright notice in the Description page of Project Settings.


#include "SnakeInstanceSubsystem.h"

void USnakeInstanceSubsystem::ChangeMode(EGameMode newGameMode){
    if(gameMode == newGameMode) return;
    gameModeChanged.Broadcast(gameMode, newGameMode);
    gameMode = newGameMode;
}