// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Attacks/Attack.h"
#include "BasicChainAttack.generated.h"

/**
 * This class is used to create a basic chain attack (IE. like the player basic attacks).
 */
UCLASS(Abstract, Blueprintable)
class ARISE_API UBasicChainAttack : public UAttack
{
	GENERATED_BODY()
	
public:

    void StartAttack() override;
    void EndAttack()   override;
};
