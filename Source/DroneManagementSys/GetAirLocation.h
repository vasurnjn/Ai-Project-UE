// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "GetAirLocation.generated.h"

class ATree;

UCLASS()
class DRONEMANAGEMENTSYS_API UGetAirLocation : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
    UGetAirLocation();
    ATree* ClosestTree;

protected:
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

    UPROPERTY(EditAnywhere, Category = "Search")
    float Radius = 2000.f;

    UPROPERTY(EditAnywhere, Category = "Altitude")
    bool bClampAltitude = true;

    UPROPERTY(EditAnywhere, Category = "Altitude", meta = (EditCondition = "bClampAltitude"))
    float MinZ = 300.f;

    UPROPERTY(EditAnywhere, Category = "Altitude", meta = (EditCondition = "bClampAltitude"))
    float MaxZ = 2000.f;

    UPROPERTY(EditAnywhere, Category = "Blackboard")
    struct FBlackboardKeySelector LocationKey;
	
    UPROPERTY(EditAnywhere)
    bool landing = false;
	
};
