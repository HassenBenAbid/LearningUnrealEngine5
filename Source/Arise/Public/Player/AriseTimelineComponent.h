// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "Components/ActorComponent.h"
#include "AriseTimelineComponent.generated.h"

class UCurveFloat;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnTimelineProgress, FVector /*New location*/)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnTimelineFinished, bool /*Is the timeline reversed?*/)

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARISE_API UAriseTimelineComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
    
    FOnTimelineProgress OnTimelineProgress;
    FOnTimelineFinished OnTimelineFinished; 

	UAriseTimelineComponent();

    UFUNCTION()
    void TimelineProgress(float value);                                                 //This is where the timeline actually progress.
    void StartTimeline();                                                               //We call this when we want the timeline to start.
    void StartTimelineReverse();                                                        //We start the same timeline but in reverse.
    void SetupTimeline(FVector startLocation, FVector endLocation);                     //We setup all the necessary properties for the timeline.

    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override; //Called every frame.

protected:

    virtual void BeginPlay() override;
    
private:	

    void ReverseTimeline();

    UPROPERTY(EditAnywhere, Category = Timeline)
    UCurveFloat* FloatCurve;

    UPROPERTY(EditAnywhere, Category = Timeline)
    float Speed;

    FTimeline TimelineCurve;
    FVector   StartLocation;
    FVector   EndLocation;
    float     LerpTime;
    bool      TimelineOnGoing;
    bool      TimelineWasReversed;
};
