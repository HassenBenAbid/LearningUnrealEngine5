// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/MainCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "Core/Components/HealthComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "Core/GameManager.h"
#include "NiagaraComponent.h"

// Sets default values
AMainCharacter::AMainCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    //We setup the player camera.
    this->CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    this->CameraBoom->SetupAttachment(this->GetRootComponent());
    this->CameraBoom->TargetArmLength         = this->DEFAULT_CAMERA_DISTANCE;
    this->CameraBoom->bUsePawnControlRotation = true; //We want the camera to follow the rotation of the player.

    //We setup the main camera and attach it to the spring arm component so that it's always adjusting to the player rotation.
    this->FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    this->FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    this->FollowCamera->bUsePawnControlRotation = false;

    this->bUseControllerRotationYaw = false; //Prevent the character from rotating with the camera.

    //We setup the niagara system which will then take the teleportation effect.
    this->TeleportEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("TeleportationEffect"));
    this->TeleportEffect->SetupAttachment(this->GetRootComponent());
    this->TeleportEffect->SetAutoActivate(false);

    //When we move, we want the character to rotate toward that direction.
    this->GetCharacterMovement()->bOrientRotationToMovement = true;
    this->GetCharacterMovement()->RotationRate              = FRotator(0.f, 600.f, 0.f);

    //We implement OnObjectHit feedbacks.
    this->GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &AMainCharacter::OnObjectHit);

    //We initialize our custom timeline component.
    this->TimelineComponent = CreateDefaultSubobject<UAriseTimelineComponent>(TEXT("AriseTimeline"));

    //
    this->AttacksComponent = CreateDefaultSubobject<UAttacksComponent>(TEXT("AttacksComponent"));

    //Setting up the weapon collider.
    this->WeaponCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponCollider"));
    this->WeaponCollider->SetupAttachment(this->GetMesh(), FName("WeaponSocket"));
    //Set what channels should the weapon register hits.
    this->WeaponCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    this->WeaponCollider->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
    this->WeaponCollider->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    this->WeaponCollider->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
    //Try to deal damage when the weapon actually hits something.
    this->WeaponCollider->OnComponentBeginOverlap.AddDynamic(this, &AMainCharacter::OnWeaponHit);
}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();

    this->PlayerAnim = this->GetMesh()->GetAnimInstance();
    if (!this->PlayerAnim) UE_LOG(LogTemp, Error, TEXT("There's no animation instance for %s"), *this->GetName());

    //We set the different params of the custom timeline component.
    auto startLocation = FVector(0, this->FollowCamera->FieldOfView, 0);
    auto endLocation = FVector(0, this->FollowCamera->FieldOfView - this->TeleportZoomEffectIntensity, 0);
    this->TimelineComponent->SetupTimeline(startLocation, endLocation);

    //We want some zoom in and zoom out effect at the start and at the end of the teleportation.
    this->TimelineComponent->OnTimelineProgress.AddLambda([this](FVector newValue) { this->FollowCamera->SetFieldOfView(newValue.Y); });
    //When the zomm in finishes, we then teleport the player, disable the teleportation effect and reverse the timeline (zoom out effect).
    this->TimelineComponent->OnTimelineFinished.AddLambda([this](bool wasReversed)
    {
        if (!wasReversed)
        {
            this->TeleportTo(this->TeleportTargetPosition, this->GetActorRotation());
            this->TimelineComponent->StartTimelineReverse();
            this->TeleportEffect->Deactivate();
        }
    });

    //Initialize attacks component.
    this->AttacksComponent->OnDealingDamageStarted(EAttackType::EAT_BasicAttack, [this] {this->EnableWeaponCollision(true); });
    this->AttacksComponent->OnDealingDamageStopped(EAttackType::EAT_BasicAttack, [this] {this->EnableWeaponCollision(false);});
    
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
                                                                                      
}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);   

    //Binding the actions.
    PlayerInputComponent->BindAction("Jump"       , IE_Pressed, this, &AMainCharacter::TeleportAbility);
    PlayerInputComponent->BindAction("BasicAttack", IE_Pressed, this, &AMainCharacter::BasicAttack);
    
    //Binding the axis.
    PlayerInputComponent->BindAxis("Forward", this, &AMainCharacter::MoveForward);
    PlayerInputComponent->BindAxis("Right"  , this, &AMainCharacter::MoveRight);
    PlayerInputComponent->BindAxis("Pitch"  , this, &APawn::AddControllerPitchInput);
    PlayerInputComponent->BindAxis("Yaw"    , this, &APawn::AddControllerYawInput);
}

void AMainCharacter::MoveForward(float value) 
{
    if (this->Controller == nullptr || value == 0.0f) return;                             //We make sure that the player has a controller and has actually moved.

    const auto direction = FRotationMatrix(this->GetYawRotation()).GetUnitAxis(EAxis::X); //We find which way is forward.            
    AddMovementInput(direction, value * this->MovementSpeed);                             //Then using the direction that we got and the specified value, we move forward.
}

void AMainCharacter::MoveRight(float value) 
{
    if (this->Controller == nullptr || value == 0.0f) return;                             //We make sure that the player has a controller and has actually moved.

    const auto direction = FRotationMatrix(this->GetYawRotation()).GetUnitAxis(EAxis::Y); //We find which way is right.              
    AddMovementInput(direction, value * this->MovementSpeed);                             //Then using the direction that we got and the specified value, we move forward.
}

void AMainCharacter::TeleportAbility() 
{
    auto hitResult       = FHitResult();
    auto currentPosition = this->GetActorLocation();
    auto targetPosition  = currentPosition + this->GetActorForwardVector() * this->TeleportDistance;
    //this->GetCapsuleComponent()->GetScaledCapsuleRadius();

    //We prevent the player from teleporting through walls (TO DO: in the future I should make this even better to ignore enemies).
    if (this->GetWorld()->LineTraceSingleByChannel(hitResult, currentPosition, targetPosition, this->CollisionChannel.GetValue())) 
        this->TeleportTargetPosition = hitResult.Location;
    else 
        this->TeleportTargetPosition = targetPosition;
     

    this->TimelineComponent->StartTimeline(); //We start the timeline where some zooming effect will happen.
    this->TeleportEffect->Activate(true);     //We also start the teleportation special effect.
}

void AMainCharacter::OnObjectHit(UPrimitiveComponent* hitComponent, AActor* otherActor, UPrimitiveComponent* otherComp, FVector normalImpulse, const FHitResult& hit)
{
    UE_LOG(LogTemp, Warning, TEXT("Hit"));

    this->GetCharacterMovement()->Velocity = this->GetVelocity() * -1;
}

void AMainCharacter::OnWeaponHit(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult)
{
    if (otherActor == nullptr || otherActor == this) return; //If we didn't hit anyone, we stop here.

    if (otherActor->GetComponentByClass(UHealthComponent::StaticClass()) != nullptr)
        UGameplayStatics::ApplyDamage(otherActor, this->AttacksComponent->GetCurrentAttackDamage(), this->GetController(), this, this->AttacksComponent->GetCurrentAttackDamageType());
}

void AMainCharacter::BasicAttack()                       { this->AttacksComponent->PlayAttack(EAttackType::EAT_BasicAttack); }
void AMainCharacter::EnableWeaponCollision(bool enable)  { this->WeaponCollider->SetCollisionEnabled(enable ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision); }

FRotator AMainCharacter::GetYawRotation() { return FRotator(0.f, this->Controller->GetControlRotation().Yaw, 0.f); }

