// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HasHealth.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)
class UHasHealth : public UInterface
{
	GENERATED_BODY()
};

/**
 * We implement this interface for any object that can be damaged.
 */
class ARISE_API IHasHealth
{
	GENERATED_BODY()

public:

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
    float GetHealthPercentage();
    
    virtual inline void ChangeHealth(int value) { this->CurrentHealth += value; }

protected:
    int CurrentHealth;
    int MaxHealth;
};
