// Fill out your copyright notice in the Description page of Project Settings.


#include "CheckStorage.h"
#include "AIController.h"
#include "DroneAi.h"


bool UCheckStorage::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const {


	const AAIController* AICon = OwnerComp.GetAIOwner();
	if (!AICon) return false;

	const APawn* AIPawn = AICon->GetPawn();
	if (!AIPawn) return false;

	const ADroneAi* Drone = Cast<ADroneAi>(AIPawn);
	if (!Drone) return false;

	if (Drone->Storage > 0) {
		return true;
	}
	else {
		return false;
	}
}


