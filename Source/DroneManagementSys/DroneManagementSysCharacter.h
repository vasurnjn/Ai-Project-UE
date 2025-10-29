// Copyright Epic Games, Inc.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "DroneManagementSysCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;

/**
 * Drone-like flying character where:
 * - W/S: forward/back (in facing direction)
 * - A/D: strafe
 * - Q/E: smoothly rotate actor yaw
 * - Mouse: look/pitch camera only
 * - No auto facing to movement
 */
UCLASS()
class  ADroneManagementSysCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ADroneManagementSysCharacter();

protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Input handlers
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void TurnYaw(const FInputActionValue& Value);
	void Ascend(const FInputActionValue& Value);
	void Descend(const FInputActionValue& Value);

protected:
	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> FollowCamera;

public:
	// === Controls ===
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Controls")
	float TurnInputYawSpeedDegPerSec = 240.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Controls")
	float MouseYawSpeedDegPerSec = 360.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Controls")
	float LookRatePitch = 1.f;

	// === Input Assets ===
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> TurnAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> AscendAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> DescendAction;


	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};
