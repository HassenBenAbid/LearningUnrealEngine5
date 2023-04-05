// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AriseTimelineComponent.h"
#include "Components/TimelineComponent.h"

// Sets default values for this component's properties
UAriseTimelineComponent::UAriseTimelineComponent()
{
	PrimaryComponentTick.bCanEverTick = true; //Tick this component every frame.
}

void UAriseTimelineComponent::BeginPlay() 
{
    Super::BeginPlay();

    this->TimelineWasReversed = false;

    if (this->FloatCurve)
    {
        FOnTimelineFloat timelineProgress;
        timelineProgress.BindUFunction(this, FName("TimelineProgress"));
        this->TimelineCurve.AddInterpFloat(this->FloatCurve, timelineProgress);
    }
    else UE_LOG(LogTemp, Error, TEXT("AriseTimelineComponent is missing a UCurveFloat."));
}

void UAriseTimelineComponent::SetupTimeline(FVector startLocation, FVector endLocation)
{
    this->StartLocation   = startLocation;
    this->EndLocation     = endLocation;
    this->TimelineOnGoing = false;
    this->TimelineCurve.SetLooping(false);
}

void UAriseTimelineComponent::StartTimeline() 
{
    this->TimelineCurve.PlayFromStart();
    this->TimelineOnGoing = true;
    this->LerpTime        = 0;
}

void UAriseTimelineComponent::StartTimelineReverse()
{
    this->ReverseTimeline();
    this->StartTimeline();

    this->TimelineWasReversed = true;
}

void UAriseTimelineComponent::ReverseTimeline()
{
    auto temp = this->StartLocation;
    this->StartLocation = this->EndLocation;
    this->EndLocation   = temp;
}

void UAriseTimelineComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    this->TimelineProgress(DeltaTime);
}

void UAriseTimelineComponent::TimelineProgress(float value)
{
    if (this->TimelineOnGoing) 
    {
        this->LerpTime += this->Speed * value;
        this->LerpTime = FMath::Clamp(this->LerpTime, 0, 1.f);

        auto currentLocation = FMath::Lerp(this->StartLocation, this->EndLocation, this->LerpTime);      //We get the new location.
        this->OnTimelineProgress.Broadcast(currentLocation);                                             //And then we pass it to all the functions that are going to use that value.
        if (currentLocation == this->EndLocation)                                                        
        {
            if (this->TimelineWasReversed) this->ReverseTimeline(); //If this timeline was reversed, we reset it.
            auto oldReversed          = this->TimelineWasReversed;
            this->TimelineWasReversed = false;
            this->TimelineOnGoing     = false;

            this->OnTimelineFinished.Broadcast(oldReversed);        //We broadcast that the timeline has finished, specifying also whether this was a normal or a reversed run.
        }
    }
}

