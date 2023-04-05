// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Attacks/Attack.h"

UAttack::UAttack() { this->ComboCounter = 0; }

void UAttack::ChainCombo()
{
    auto currentAnim = this->GetUsedAnimInstance();
    if (currentAnim == nullptr) return;

    this->ComboCounter = this->ComboCounter >= this->Attacks.Num() - 1 ? 0 : this->ComboCounter + 1; //If the combo is finished, we restart the combo counter.
    //If the combo has just ended then we should make the player wait for the last attack animation to completely finish before being able to attack again.
    if (this->ComboCounter > 0) this->IsPlaying = false;
}

UAnimInstance* const UAttack::GetUsedAnimInstance() const
{
    if (AnimInstance == nullptr) UE_LOG(LogTemp, Error, TEXT("There's no defined animation instance for %s attack."), *this->GetName()); 
    return this->AnimInstance;
}

void UAttack::StartDealingDamage() { this->DealDamage = true;  this->OnDamageOn.Broadcast(); }
void UAttack::StopDealingDamage()  { this->DealDamage = false; this->OnDamageOff.Broadcast(); }