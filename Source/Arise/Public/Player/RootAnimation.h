// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "RootAnimation.generated.h"

/**
 * 
 */
UCLASS()
class ARISE_API URootAnimation : public UAnimInstance
{
	GENERATED_BODY()

public:

    //
    UPROPERTY(BlueprintReadOnly, Category = AnimationProperties)
    float MovementSpeed;

    UPROPERTY(BlueprintReadOnly, Category = AnimationProperties)
    bool IsMoving;

private:

    //If the speed of the character is above this value, he will start moving.
    inline static const float MIN_SPEED_MOVEMENT = 0.05f;

    //This is where we initialize all the animation properties before the animation actually starts.
    virtual void NativeInitializeAnimation() override;

    //This is where we update our animation properties every frame.
    virtual void NativeUpdateAnimation(float deltaSeconds) override;

    //The pawn that's connected to this animation blueprint.
    UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
    APawn* ConnectedPawn;
	
};
