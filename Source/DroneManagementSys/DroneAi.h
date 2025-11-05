// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DroneAi.generated.h"

UCLASS()
class DRONEMANAGEMENTSYS_API ADroneAi : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADroneAi();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere)
	int Storage = 4;

	UPROPERTY(VisibleAnywhere)
	int64 RowId = -1;

	int32 TreesExtinguished = 0;

	UFUNCTION(BlueprintCallable)
	void DisplayDB();

	
	
};
