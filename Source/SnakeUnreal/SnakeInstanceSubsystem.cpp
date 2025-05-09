// Fill out your copyright notice in the Description page of Project Settings.


#include "SnakeInstanceSubsystem.h"

void USnakeInstanceSubsystem::ChangeMode(EGameMode newGameMode){
    if(gameMode == newGameMode) return;
    gameModeChanged.Broadcast(gameMode, newGameMode);
    gameMode = newGameMode;
}

void USnakeInstanceSubsystem::SetScores(TArray<int> score){
    scores = score;
}

void USnakeInstanceSubsystem::NextMapID(){
    mapID++;
}

void USnakeInstanceSubsystem::SetMapID(int id){
    mapID = id;
}