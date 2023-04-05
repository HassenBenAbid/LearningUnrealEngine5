// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Player/MainCharacter.h"
#include "LevelManager.generated.h"

/**
 * 
 */
UCLASS()
class ARISE_API ALevelManager : public AGameModeBase
{
	GENERATED_BODY()

public:

    AMainCharacter* const GetPlayer();	

private: 
    
    UPROPERTY()
    AMainCharacter* MainPlayer;
};
