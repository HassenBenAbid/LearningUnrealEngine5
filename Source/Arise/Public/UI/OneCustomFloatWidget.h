// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OneCustomFloatWidget.generated.h"

/**
 * 
 */
UCLASS()
class ARISE_API UOneCustomFloatWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
    
    inline void SetCustomFloat(float value) { this->CustomFloat = value; }

private:

    UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
    float CustomFloat;
};
