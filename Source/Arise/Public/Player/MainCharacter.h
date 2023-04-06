// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AriseTimelineComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Core/Components/AttacksComponent.h"
#include "GameFramework/Character.h"
#include "MainCharacter.generated.h"

UCLASS()
class ARISE_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

public:

	// Sets default values for this character's properties
	AMainCharacter();

    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

    inline static const float DEFAULT_CAMERA_DISTANCE  = 400.f; //The default distance between the camera and the player.
    inline static const float MIN_KEY_TIME_HELD        = 0.5f;  //The amount of time the key responsible for guided teleportation to be held before it triggers.

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = true))
    class USpringArmComponent* CameraBoom; //This will allow us to make sure that the camera is always positioned behind the player.

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = true))
    class UCameraComponent* FollowCamera;  //This is the camera that will follow the player.

    UPROPERTY(VisibleAnywhere)
    class UBoxComponent* WeaponCollider;

    UPROPERTY(EditDefaultsOnly, Category = Teleport, meta = (AllowPrivateAccess = true))
    UAriseTimelineComponent* TimelineComponent; //

    UPROPERTY(EditDefaultsOnly, Category = Teleport)
    UNiagaraComponent* TeleportEffect; //This will allow us to enable some particle effects when the player teleports.

    UPROPERTY(EditAnywhere, Category = Movement)
    TEnumAsByte<ECollisionChannel> CollisionChannel; //

    UPROPERTY(EditDefaultsOnly, Category = Attacks)
    UAttacksComponent* AttacksComponent; //This will take care of storing the different attacks and starting them.


    UPROPERTY(EditAnywhere, Category = Teleport)
    float              TeleportDistance; //The distance that the player can teleport.

    UPROPERTY(EditAnywhere, Category = Teleport, meta = (AllowPrivateAccess = true))
    float              TeleportZoomEffectIntensity; //

    UPROPERTY(EditAnywhere, Category = movement, meta = (AllowPrivateAccess = true))
    float MovementSpeed;

    UPROPERTY() UAnimInstance* PlayerAnim;

    FVector TeleportTargetPosition;

    void BasicAttack(); //
   
    void MoveForward(float value); //Move the player forward/backward.
    void MoveRight(float value);   //Move the player right/left.
    void TeleportAbility();        //This is responsible for the logic behind player teleportation ability.

    //We use this when the character hits a static object (like a wall) to actually eliminate the sliding effect when he keeps running into it.
    UFUNCTION()
    void OnObjectHit(UPrimitiveComponent* hitComponent, AActor* otherActor, UPrimitiveComponent* otherComp, FVector normalImpulse, const FHitResult& hit);

    UFUNCTION()
    void OnWeaponHit(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult);

    void EnableWeaponCollision(bool enable); //Takes care of enabling the weapon collision when we want it to actually deal damage.

    /*** Utils ***/

    FRotator GetYawRotation(); //We get the current YawRotation using the controller.
};
