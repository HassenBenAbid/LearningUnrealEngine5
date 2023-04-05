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

    this->WidgetComponent = CreateDefaultSubobject<UFocusAtTargetWidgetComponent>(TEXT("WidgetComponent"));
    this->WidgetComponent->SetupAttachment(this->MeshComponent);

    this->HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
}

void ABasicEnemy::BeginPlay()
{
    Super::BeginPlay();

    this->HealthComponent->InitializeHealthUI(*this->WidgetComponent);
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

//float ABasicEnemy::GetHealthPercentage() {return this->CurrentHealth / this->MaxHealth; }

