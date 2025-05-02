// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SnakeInstanceSubsystem.generated.h"

UENUM(BlueprintType)
enum class EGameMode : uint8{
	NoState,
	MenuState,
	PlayState,
	PauseState,
	OnePlayer,
	TwoPlayer,
	VsAI
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnGameModeChanged, EGameMode, OldGameMode, EGameMode, NewGameMode);
/**
 * 
 */
UCLASS()
class SNAKEUNREAL_API USnakeInstanceSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	private:
	EGameMode gameMode = EGameMode::NoState;

	public:

	UPROPERTY(BlueprintAssignable, Category= "GameMode")
	FOnGameModeChanged gameModeChanged;

	UFUNCTION(BlueprintCallable)
	FORCEINLINE EGameMode GetGameMode() const {return gameMode;}

	UFUNCTION(BlueprintCallable)
	void ChangeMode(EGameMode NewGameMode);
};
