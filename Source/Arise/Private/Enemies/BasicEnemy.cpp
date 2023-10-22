// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/BasicEnemy.h"
#include "Blueprint/UserWidget.h"
#include "Components/WidgetComponent.h"
#include "Core/Components/HealthComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ABasicEnemy::ABasicEnemy()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    this->MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    this->MeshComponent->SetupAttachment(this->GetRootComponent());

    this->HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
    this->HealthComponent->SetupAttachment(this->MeshComponent);
}

void ABasicEnemy::BeginPlay()
{
    Super::BeginPlay();
}

// Called every frame
void ABasicEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ABasicEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

