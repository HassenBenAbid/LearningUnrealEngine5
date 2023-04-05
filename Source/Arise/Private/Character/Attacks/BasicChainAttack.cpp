// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Attacks/BasicChainAttack.h"

void UBasicChainAttack::StartAttack()
{
    auto currentAnim = this->GetUsedAnimInstance();
    if (currentAnim == nullptr) return;

    if (!this->IsPlaying)
    {
        this->IsPlaying = true;
        currentAnim->Montage_Play(this->Attacks[ComboCounter]);
        if (this->ComboCounter == 0) this->OnAttackStarted.Broadcast();
    }
}

void UBasicChainAttack::EndAttack()
{
    auto currentAnim = this->GetUsedAnimInstance();
    if (currentAnim == nullptr) return;

    this->ComboCounter = 0;
    this->IsPlaying    = false;
    this->OnAttackEnded.Broadcast();
}
