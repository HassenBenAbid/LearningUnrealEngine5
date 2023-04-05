// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Character/Attacks/Attack.h"
#include "AttacksComponent.generated.h"

UENUM()
enum EAttackType
{
    EAT_NoAttack        = -1 UMETA(DisplayName = "No Attack"),
    EAT_BasicAttack     = 0  UMETA(DisplayName = "Basic Attack"),
    EAT_SecondaryAttack = 1  UMETA(DisplayName = "Secondary Attack"),
    EAT_SpecialAttack1  = 2  UMETA(DisplayName = "Special Attack 1"),
    EAT_SpecialAttack2  = 3  UMETA(DisplayName = "Special Attack 2"),
    EAT_UltimateAttack  = 4  UMETA(DisplayName = "Ultimate Attack")
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARISE_API UAttacksComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UAttacksComponent();

    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    void PlayAttack(EAttackType attackType);

    /*Whenever the specified attack type start dealing damage, we call the specified function*/
    void OnDealingDamageStarted(EAttackType attackType, TFunction<void()> functionToCall);

    /*Whenever the specified attack type stop dealing damage, we call the specified function*/
    void OnDealingDamageStopped(EAttackType attackType, TFunction<void()> functionToCall);

    inline bool IsAttacking()                                     { return this->CurrentAttackType != EAttackType::EAT_NoAttack; }
    inline void SetAnimationInstance(UAnimInstance* animInstance) { this->AnimationInstance = animInstance; }
    
    UFUNCTION(BlueprintCallable) inline UAttack* const GetCurrentAttack() const {return this->AttacksIntances[this->CurrentAttackType]; }

protected:

	virtual void BeginPlay() override;

private:	

    UPROPERTY(EditDefaultsOnly)    TArray<TSubclassOf<UAttack>> AttacksTypes;      //All the different types of attacks used by this component.
    EAttackType                  CurrentAttackType; //The index of the current attack being played, if there's no attack being played then this will be set to -1.
    
    UPROPERTY() TArray<UAttack*> AttacksIntances;   //The list containing isntances of all the attacks used by this component (same order as AttacksTypes)
    UPROPERTY() UAnimInstance*   AnimationInstance; //The animation instance that will be using the different attacks animations.

    void AttackEnded(); //This gets called when the current attack ends.
		
};
