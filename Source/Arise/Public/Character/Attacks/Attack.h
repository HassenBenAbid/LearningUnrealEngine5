// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Attack.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnEventNoParam)

/**
 * 
 */
UCLASS(Abstract, Blueprintable)
class ARISE_API UAttack : public UObject
{
	GENERATED_BODY()

public:

    UAttack();

    FOnEventNoParam OnAttackStarted;
    FOnEventNoParam OnAttackEnded;
    FOnEventNoParam OnDamageOn;
    FOnEventNoParam OnDamageOff;
    
    inline virtual void InitAttack(UAnimInstance* animInstance) { this->AnimInstance = animInstance; }
    
    UFUNCTION(BlueprintCallable) virtual void StartAttack() PURE_VIRTUAL(UAttack::StartAttack,);
    UFUNCTION(BlueprintCallable) virtual void EndAttack()   PURE_VIRTUAL(UAttack::EndAttack,);
    UFUNCTION(BlueprintCallable) virtual void ChainCombo(); //Some attacks can have different parts, this would allow us to go to the next part.
    
    //These will care of when exactly during animations should we deal damage.
    UFUNCTION(BlueprintCallable) virtual void StartDealingDamage();
    UFUNCTION(BlueprintCallable) virtual void StopDealingDamage();

    UFUNCTION(BlueprintCallable) inline bool                     CanDealDamage()   const  { return this->DealDamage; }
    UFUNCTION(BlueprintCallable) inline int                      GetAttackDamage() const  { return this->AttackDamage;}
    UFUNCTION(BlueprintCallable) inline TSubclassOf<UDamageType> GetDamageType()   const  { return this->DamageType; }
protected:

    UPROPERTY(EditDefaultsOnly) TSubclassOf<UDamageType> DamageType;   //What type of damage this attack is going to deal ?
    UPROPERTY(EditDefaultsOnly) TArray<UAnimMontage*>    Attacks;      //This store all the different animation parts of this attack (IMPORTANT: list order is used to know what animation to play first).  
    UPROPERTY(EditAnywhere)     int                      AttackDamage; //The base damage that this attack would deal. 
    
    bool IsPlaying;                                                    //Is this attack currently being played ?
    int  ComboCounter;                                                 //We store the current combo index, when the character has already played all the animation this get reset to 0;
    bool DealDamage;                                                   //Can we currently deal damage?     

    //We return the animation instance that's connected to this attack and if there's non show an error.
    UAnimInstance* const GetUsedAnimInstance() const;

private:

    UPROPERTY() UAnimInstance* AnimInstance; //The animation instance that's connected to this attack.
	
};
