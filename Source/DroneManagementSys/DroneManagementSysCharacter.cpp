// Copyright Epic Games, Inc.

#include "DroneManagementSysCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

// Enhanced Input
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "InputActionValue.h"

ADroneManagementSysCharacter::ADroneManagementSysCharacter()
{
	

	// === Flying setup ===
	UCharacterMovementComponent* Move = GetCharacterMovement();
	Move->SetMovementMode(MOVE_Walking);
	Move->GravityScale = 1.0f;
	Move->MaxFlySpeed = 1200.f;
	Move->BrakingDecelerationFlying = 6000.f;
	Move->BrakingFriction = 0.2f;
	Move->bUseSeparateBrakingFriction = true;

	// No automatic facing
	Move->bOrientRotationToMovement = false;
	Move->bUseControllerDesiredRotation = false;

	// Actor rotation will drive direction manually
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	// === Camera rig ===
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.f;
	CameraBoom->bUsePawnControlRotation = true; // camera follows rotation

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
}

void ADroneManagementSysCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (UCharacterMovementComponent* Move = GetCharacterMovement())
	{
		Move->SetMovementMode(MOVE_Flying);
	}

	// Add input mapping context
	if (APlayerController* PC = Cast<APlayerController>(Controller))
	{
		if (ULocalPlayer* LP = PC->GetLocalPlayer())
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsys = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
			{
				if (DefaultMappingContext)
				{
					Subsys->AddMappingContext(DefaultMappingContext, 0);
				}
			}
		}
	}
}

void ADroneManagementSysCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (MoveAction)   EIC->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ADroneManagementSysCharacter::Move);
		if (LookAction)   EIC->BindAction(LookAction, ETriggerEvent::Triggered, this, &ADroneManagementSysCharacter::Look);
		if (TurnAction)   EIC->BindAction(TurnAction, ETriggerEvent::Ongoing, this, &ADroneManagementSysCharacter::TurnYaw);
		if (AscendAction) EIC->BindAction(AscendAction, ETriggerEvent::Ongoing, this, &ADroneManagementSysCharacter::Ascend);
		if (DescendAction)EIC->BindAction(DescendAction, ETriggerEvent::Ongoing, this, &ADroneManagementSysCharacter::Descend);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("'%s' missing EnhancedInputComponent."), *GetNameSafe(this));
	}
}

/* ================= Input Handlers ================= */

void ADroneManagementSysCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D Axes = Value.Get<FVector2D>();

	if (Axes.Y != 0.f)
		AddMovementInput(GetActorForwardVector(), Axes.Y);

	if (Axes.X != 0.f)
		AddMovementInput(GetActorRightVector(), Axes.X);
}

void ADroneManagementSysCharacter::Look(const FInputActionValue& Value)
{
 const FVector2D Axes = Value.Get<FVector2D>(); // X=Yaw, Y=Pitch

    if (Axes.X != 0.f)
    {
        // Yaw the CAMERA (controller), not the actor
        AddControllerYawInput(Axes.X * MouseYawSpeedDegPerSec * GetWorld()->GetDeltaSeconds());
    }

    if (Axes.Y != 0.f)
    {
        // Pitch the CAMERA (controller)
        AddControllerPitchInput(Axes.Y * LookRatePitch * GetWorld()->GetDeltaSeconds());
    }
}

void ADroneManagementSysCharacter::TurnYaw(const FInputActionValue& Value)
{
	const float Axis = Value.Get<float>();   // +1 (E) / -1 (Q) or similar
	if (Axis == 0.f) return;

	const float DeltaYaw = Axis * TurnInputYawSpeedDegPerSec * GetWorld()->GetDeltaSeconds();
	AddActorWorldRotation(FRotator(0.f, DeltaYaw, 0.f));

	// Keep camera aligned to the actor yaw (so camera doesn't lag behind)
	if (AController* C = Controller)
	{
		C->SetControlRotation(GetActorRotation());
	}
}

void ADroneManagementSysCharacter::Ascend(const FInputActionValue& Value)
{
	const float Thrust = Value.Get<float>(); // e.g., 1 while held
    if (Thrust != 0.f)
        AddMovementInput(GetActorUpVector(), Thrust);
}

void ADroneManagementSysCharacter::Descend(const FInputActionValue& Value)
{
	const float Thrust = Value.Get<float>();
	if (Thrust != 0.f)
		AddMovementInput(-GetActorUpVector(), Thrust);
}
