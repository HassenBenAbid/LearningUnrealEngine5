// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Core/Components/HealthComponent.h"
#include "Components/WidgetComponent.h"
#include "UI/FocusAtTargetWidgetComponent.h"
#include "BasicEnemy.generated.h"

UCLASS()
class ARISE_API ABasicEnemy : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABasicEnemy();

    // Called every frame
    virtual void Tick(float DeltaTime) override;
    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

    UPROPERTY(EditDefaultsOnly) 
    UStaticMeshComponent* MeshComponent;

    UPROPERTY(EditDefaultsOnly) 
    UFocusAtTargetWidgetComponent* WidgetComponent;

    UPROPERTY(EditAnywhere)     
    UHealthComponent* HealthComponent;

};
