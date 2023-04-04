// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/MainCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "NiagaraFunctionLibrary.h"
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

    //We implement OnHit feedbacks.
    this->GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &AMainCharacter::OnHit);

    //We initialize our custom timeline component.
    this->TimelineComponent = CreateDefaultSubobject<UAriseTimelineComponent>(TEXT("AriseTimeline"));
}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();

    this->MainController = this->GetWorld()->GetFirstPlayerController(); //We get the main controller used by the player.
    this->PlayerAnim     = this->GetMesh()->GetAnimInstance();
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

    this->ComboCounter = 0;
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
    PlayerInputComponent->BindAction("BasicAttack", IE_Pressed, this, &AMainCharacter::StartAttack);
    
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

void AMainCharacter::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    UE_LOG(LogTemp, Warning, TEXT("Hit"));

    this->GetCharacterMovement()->Velocity = this->GetVelocity() * -1;
}

void AMainCharacter::StartAttack()
{
    if (!this->IsAttacking)
    {
        this->IsAttacking = true;
        this->PlayerAnim->Montage_Play(this->Attacks[ComboCounter]);
    }
}

void AMainCharacter::EndAttack()
{
    this->ComboCounter = 0;
    this->IsAttacking  = false;
}

void AMainCharacter::AllowComboAttack()
{
    this->ComboCounter = this->ComboCounter >= this->Attacks.Num() - 1 ? 0 : this->ComboCounter + 1; //If the combo is finished, we restart the combo counter.
    //If the combo has just ended then we should make the player wait for the last attack animation to completely finish before being able to attack again.
    if (this->ComboCounter > 0) this->IsAttacking  = false;
}

FRotator AMainCharacter::GetYawRotation() { return FRotator(0.f, this->Controller->GetControlRotation().Yaw, 0.f); }

