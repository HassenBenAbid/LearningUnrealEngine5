// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Attacker.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)
class UAttacker : public UInterface
{
	GENERATED_BODY()
};

/**
 * We implement this interface when we want a character to have some attack functionalities.
 */
class ARISE_API IAttacker
{
	GENERATED_BODY()

public:

    virtual void StartAttack()      = 0;
    virtual void EndAttack()        = 0;
    virtual void AllowComboAttack() = 0; //This is called whenever we want to allow the character to chain another attack before the previous one has completely finished.

protected:
    
    bool IsAttacking;  //This is set to true when this character is currently attacking.
    int  DamageValue;  //The damage that the basic attack deals.
    int  ComboCounter; //We store the current combo index, when the character has already played all the animation this get reset to 0;
};
