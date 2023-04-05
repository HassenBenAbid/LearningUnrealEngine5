// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/LevelManager.h"
#include "GameFramework/PlayerController.h"

AMainCharacter* const ALevelManager::GetPlayer()
{
    this->MainPlayer = this->MainPlayer == nullptr ? Cast<AMainCharacter>(this->GetWorld()->GetFirstPlayerController()->GetPawn()) : this->MainPlayer;
    return this->MainPlayer;
}