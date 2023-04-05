// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Components/AttacksComponent.h"

// Sets default values for this component's properties
UAttacksComponent::UAttacksComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UAttacksComponent::BeginPlay()
{
	Super::BeginPlay();

    //We make sure to get the animation instance from the skeletal mesh else print an error.
    auto meshComponent = Cast<USkeletalMeshComponent>(GetOwner()->GetComponentByClass(USkeletalMeshComponent::StaticClass()));
    if (meshComponent != nullptr)
        this->AnimationInstance = meshComponent->GetAnimInstance();
    else
        UE_LOG(LogTemp, Error, TEXT("You are using an attack component on %s without any skeletal mesh component."), *this->GetOwner()->GetName())

    for (auto attack : this->AttacksTypes)
    {
        auto attackInstance = NewObject<UAttack>(this, attack);
        attackInstance->InitAttack(this->AnimationInstance);
        attackInstance->OnAttackEnded.AddUObject(this, &UAttacksComponent::AttackEnded); //We make sure to subscribe to the attack so that we get notified when it ends.
        this->AttacksIntances.Add(attackInstance);
    }

    this->CurrentAttackType = EAttackType::EAT_NoAttack;
}

// Called every frame
void UAttacksComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UAttacksComponent::PlayAttack(EAttackType attackType)
{
    if (this->CurrentAttackType == -1 || this->CurrentAttackType == attackType)
    {
        this->AttacksIntances[attackType]->StartAttack();
        this->CurrentAttackType = attackType;
    }
}

void UAttacksComponent::AttackEnded()
{
    this->CurrentAttackType = EAttackType::EAT_NoAttack;
}

void UAttacksComponent::OnDealingDamageStarted(EAttackType attackType, TFunction<void()> functionToCall)
{
    this->AttacksIntances[attackType]->OnDamageOn.AddLambda([functionToCall] {functionToCall();});
}

void UAttacksComponent::OnDealingDamageStopped(EAttackType attackType, TFunction<void()> functionToCall)
{
    this->AttacksIntances[attackType]->OnDamageOff.AddLambda([functionToCall] {functionToCall(); });
}