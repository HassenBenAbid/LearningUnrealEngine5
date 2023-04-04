#pragma once

#include "GameFramework/Pawn.h"

namespace PawnUtils 
{
    //Get the horizontal speed of the specified pawn.
    float GetHorizontalSpeed(const APawn* pawn) 
    {
        auto pawnVelocity = pawn->GetVelocity();
        return FVector(pawnVelocity.X, pawnVelocity.Y, 0.0).Size();
    }
}
