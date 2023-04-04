// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/RootAnimation.h"
#include "Utils/PawnUtils.h"

void URootAnimation::NativeInitializeAnimation() 
{
    if (this->ConnectedPawn == nullptr) this->ConnectedPawn = this->TryGetPawnOwner();
}

void URootAnimation::NativeUpdateAnimation(float deltaSeconds)
{
    if (this->ConnectedPawn == nullptr)
    {
        UE_LOG(LogTemp, Error, TEXT("An animation blueprint doesn't have a connected pawn."));
        return;
    }

    this->MovementSpeed = PawnUtils::GetHorizontalSpeed(this->ConnectedPawn);
    this->IsMoving      = this->MovementSpeed >= URootAnimation::MIN_SPEED_MOVEMENT;
}