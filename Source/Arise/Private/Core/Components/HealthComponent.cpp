// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Components/HealthComponent.h"
#include "Components/WidgetComponent.h"
#include "UI/OneCustomFloatWidget.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent() : Super()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

    //Initialize Health widget which will be showing the health bar on top of the connected pawn. 
    this->HealthWidgetComponent = CreateDefaultSubobject<UFocusAtTargetWidgetComponent>(TEXT("WidgetComponent"));
    //this->HealthWidgetComponent->SetupAttachment(this);
}

void UHealthComponent::OnRegister()
{
    Super::OnRegister();
}

// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

    //We initialize a full health.
    this->IsDead = false;
    this->CurrentHealth = this->MaxHealth;   

    //We instantiate the health widget and assign it to the widget component. 
    this->HealthWidgetComponent->SetWidgetClass(this->HealthWidget);
    this->CurrentHealthWidget = Cast<UOneCustomFloatWidget>(this->HealthWidgetComponent->GetWidget());

    this->UpdateHealthUI(); //We make sure that the health UI is set to full health.
                                          
    this->GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::TakeDamage); //We make sure to subscribe this component take damage to the owner actor.
}

// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UHealthComponent::ChangeHealth(int value)
{
    this->CurrentHealth += value;
    this->UpdateHealthUI();
}

void UHealthComponent::UpdateHealthUI()
{
    if (this->CurrentHealthWidget == nullptr) return; //If there's no health widget instance we don't update the health UI.

    this->CurrentHealthWidget->SetCustomFloat(this->GetHealthPercentage());
}

void UHealthComponent::TakeDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
    if (this->CurrentHealth <= 0) this->Die();
    else this->ChangeHealth(-Damage);
}

void UHealthComponent::Die()
{
    this->IsDead = true;
}
