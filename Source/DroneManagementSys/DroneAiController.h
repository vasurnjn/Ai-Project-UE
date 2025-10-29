// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "DroneAiController.generated.h"

/**
 * 
 */
UCLASS()
class DRONEMANAGEMENTSYS_API ADroneAiController : public AAIController
{
	GENERATED_BODY()
	
	
public:
	virtual void OnPossess(APawn* InPawn) override;

protected:

	UPROPERTY(EditAnywhere)
	UBehaviorTree* BehaviorTree;

	UPROPERTY(EditAnywhere)
	UBlackboardComponent* BlackBoardComp;
	
};
