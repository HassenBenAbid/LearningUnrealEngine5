// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/TeleportTarget.h"

// Sets default values
ATeleportTarget::ATeleportTarget()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    this->StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MainMesh"));
    this->StaticMesh->SetupAttachment(this->GetRootComponent());

    this->CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    this->CameraBoom->SetupAttachment(this->StaticMesh);
    this->CameraBoom->bUsePawnControlRotation = true;

    this->FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    this->FollowCamera->SetupAttachment(this->CameraBoom, USpringArmComponent::SocketName);
    this->FollowCamera->bUsePawnControlRotation = false;
}

void ATeleportTarget::SetupTarget(float movementSpeed, float maxDistance, float cameraDistance, APlayerController *controller) 
{
    this->CameraBoom->TargetArmLength = cameraDistance;
    this->MovingSpeed                 = movementSpeed;
    this->MaxDistance                 = 300.0f;

    //this->FollowCamera->ToggleActive();
    //this->FollowCamera->SetActive(true, true);
    controller->SetViewTargetWithBlend(this, 0.5f);
}

// Called when the game starts or when spawned
void ATeleportTarget::BeginPlay()
{
	Super::BeginPlay();
	
    this->InitialPosition = this->GetTransform().GetLocation();
}

void ATeleportTarget::Move(FVector direction, float value) 
{
    auto nextPosition = this->GetTransform().GetLocation() + (direction * value);

    //if (FVector::Distance(this->InitialPosition, nextPosition) > this->MaxDistance) return;
    this->SetActorLocation(nextPosition);
}

