// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/FocusAtTargetWidgetComponent.h"
#include "Core/LevelManager.h"

void UFocusAtTargetWidgetComponent::BeginPlay()
{
    Super::BeginPlay();
}

void UFocusAtTargetWidgetComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    if (this->HasBegunPlay())
    {
        this->LookAtPlayer();
    }
}

void UFocusAtTargetWidgetComponent::LookAtPlayer()
{
    //If there's no playe to look at, we don't do anything.
    if (!this->PlayerFound()) return;

    //We first take the direction toward the target that we want to look at.
    auto direction = this->PlayerToLookAt->GetActorLocation() - this->GetOwner()->GetActorLocation();
    direction.Z = 0.f;
    //And then we rotate this component accordingly.
    auto nextRotation = direction.Rotation();
    this->SetWorldRotation(nextRotation);
}

bool UFocusAtTargetWidgetComponent::PlayerFound()
{
    this->PlayerToLookAt = this->PlayerToLookAt != nullptr ? this->PlayerToLookAt : (Cast<ALevelManager>(this->GetWorld()->GetAuthGameMode()))->GetPlayer();
    return this->PlayerToLookAt != nullptr;
}