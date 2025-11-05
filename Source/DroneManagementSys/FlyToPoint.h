// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "FlyToPoint.generated.h"

/**
 * 
 */
UCLASS()
class DRONEMANAGEMENTSYS_API UFlyToPoint : public UBTTaskNode
{
	GENERATED_BODY()
	
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere);
	float AcceptanceRadius = 150.0f;

	UPROPERTY(EditAnywhere)
	float MaxSteerAccel = 4.0f;

	UPROPERTY(EditAnywhere)
	struct FBlackboardKeySelector DestinationKey;
public:
	UFlyToPoint();

	UPROPERTY(EditAnywhere)
	bool Replenish = false;;
};
