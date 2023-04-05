// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "Player/MainCharacter.h"
#include "FocusAtTargetWidgetComponent.generated.h"

/**
 * This is used when we want a specific widget component to be always rotated toward the player.
 */
UCLASS()
class ARISE_API UFocusAtTargetWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:

    //UFocusAtTargetWidgetComponent();

protected:

    void BeginPlay() override;
    void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:

    UPROPERTY()
    AMainCharacter* PlayerToLookAt;

    void LookAtPlayer(); //This function take taking of rotating this component toward the player.
    bool PlayerFound();  //We return whether we actually have a player to look toward or not.
	
};
