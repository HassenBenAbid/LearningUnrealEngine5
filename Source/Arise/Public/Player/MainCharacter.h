// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "AriseTimelineComponent.h"
#include "Character/Attacker.h"
#include "GameFramework/SpringArmComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/CapsuleComponent.h"
#include "MainCharacter.generated.h"

UCLASS()
class ARISE_API AMainCharacter : public ACharacter, public IAttacker
{
	GENERATED_BODY()

public:

    UPROPERTY(EditAnywhere, Category = Teleport)
    UNiagaraComponent* TeleportEffect; //This will allow us to enable some particle effects when the player teleports.

    UPROPERTY(EditAnywhere, Category = Movement)
    TEnumAsByte<ECollisionChannel> CollisionChannel; //

    UPROPERTY(EditAnywhere, Category = Attacking)
    TArray<UAnimMontage*> Attacks;    //This store all the different basic attacks of this character (the list order is used for the combo order).

	// Sets default values for this character's properties
	AMainCharacter();

    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    UFUNCTION(BlueprintCallable) void EndAttack()        override;
    UFUNCTION(BlueprintCallable) void AllowComboAttack() override;
                                 void StartAttack()      override;

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

    inline static const float DEFAULT_CAMERA_DISTANCE  = 400.f; //The default distance between the camera and the player.
    inline static const float MIN_KEY_TIME_HELD        = 0.5f;  //The amount of time the key responsible for guided teleportation to be held before it triggers.

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = true))
    USpringArmComponent* CameraBoom; //This will allow us to make sure that the camera is always positioned behind the player.

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = true))
    UCameraComponent* FollowCamera;  //This is the camera that will follow the player.

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Teleport, meta = (AllowPrivateAccess = true))
    UAriseTimelineComponent* TimelineComponent; //


    UPROPERTY(EditAnywhere, Category = Teleport)
    float              TeleportDistance; //The distance that the player can teleport.

    UPROPERTY(EditAnywhere, Category = Teleport, meta = (AllowPrivateAccess = true))
    float              TeleportZoomEffectIntensity; //

    UPROPERTY(EditAnywhere, Category = movement, meta = (AllowPrivateAccess = true))
    float MovementSpeed;


    UPROPERTY() APlayerController* MainController;
    UPROPERTY() UAnimInstance*     PlayerAnim;

    bool    IsTeleporting;
    FVector TeleportTargetPosition;
   
    void MoveForward(float value); //Move the player forward/backward.
    void MoveRight(float value);   //Move the player right/left.
    void TeleportAbility();        //This is responsible for the logic behind player teleportation ability.

    UFUNCTION()
    void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

    /*** Utils ***/

    FRotator GetYawRotation(); //We get the current YawRotation using the controller.
};
