// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UI/OneCustomFloatWidget.h"
#include "HealthComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARISE_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

    //I will need to add two delegates for when receiving damage and healing.
    
	UHealthComponent();

    UFUNCTION(BlueprintCallable) inline float GetHealthPercentage() const { return (float)this->CurrentHealth / (float)this->MaxHealth; };

    void InitializeHealthUI(class UWidgetComponent& widgetComponent); //Using the specified widget compoenent, we create the health widget and update it.
    void ChangeHealth(int value);

protected:

	virtual void BeginPlay() override;

    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:

    UPROPERTY(EditDefaultsOnly) TSubclassOf<UOneCustomFloatWidget> HealthWidget;
    UPROPERTY() UOneCustomFloatWidget* CurrentHealthWidget; //The current health widget instance that's owned by this component.


    UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true)) int  MaxHealth;
    UPROPERTY(VisibleInstanceOnly)                              int  CurrentHealth;

    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true)) bool IsDead;

    void UpdateHealthUI();

    //This function is always subscribed to the owner actor and make sure that this component is updated whenever the owner is damaged.
    UFUNCTION()
    void TakeDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

    void Die(); //This gets called whenever the current health hits 0.
		
};
