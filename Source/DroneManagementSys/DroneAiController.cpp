// Fill out your copyright notice in the Description page of Project Settings.


#include "DroneAiController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"



void ADroneAiController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    UE_LOG(LogTemp, Warning, TEXT("Hello from C++!")); 
    if (BehaviorTree)
    {
        
        
            RunBehaviorTree(BehaviorTree);
        
    }
}