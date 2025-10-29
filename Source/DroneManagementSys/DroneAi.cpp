// Fill out your copyright notice in the Description page of Project Settings.


#include "DroneAi.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ADroneAi::ADroneAi()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    bUseControllerRotationYaw = false;

    UCharacterMovementComponent* Move = GetCharacterMovement();
    Move->DefaultLandMovementMode = MOVE_Flying;   // not strictly required
    Move->SetMovementMode(MOVE_Flying);
    Move->MaxFlySpeed = 600.f;
    Move->BrakingDecelerationFlying = 2048.f;
    Move->bOrientRotationToMovement = true;
    Move->RotationRate = FRotator(0.f, 360.f, 0.f);
}

// Called when the game starts or when spawned
void ADroneAi::BeginPlay()
{
	Super::BeginPlay();
    GetCharacterMovement()->SetMovementMode(MOVE_Flying);
	
}

// Called every frame
void ADroneAi::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ADroneAi::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

