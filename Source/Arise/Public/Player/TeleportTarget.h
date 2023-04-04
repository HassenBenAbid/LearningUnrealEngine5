// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "TeleportTarget.generated.h"

UCLASS(Blueprintable)
class ARISE_API ATeleportTarget : public AActor
{
	GENERATED_BODY()
	
public:	
	//Sets default values for this actor's properties.
	ATeleportTarget();

    //
    void SetupTarget(float movingSpeed, float maxDistance, float cameraDistance, APlayerController *controller);
    
    //We use this method to move.
    void Move(FVector direction, float value);

protected:
	//Called when the game starts or when spawned.
	virtual void BeginPlay() override;

private: 

    float MovingSpeed;
    float MaxDistance;  

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
    UStaticMeshComponent* StaticMesh;

    USpringArmComponent* CameraBoom;
    UCameraComponent*    FollowCamera;

    FVector InitialPosition;
};
