// Fill out your copyright notice in the Description page of Project Settings.


#include "FlyToPoint.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GetAirLocation.h"
#include "Tree.h"
#include "DroneAi.h"
#include "DatabaseSubsystem.h"

UFlyToPoint::UFlyToPoint()
{
	NodeName = TEXT("Flu to Point");
	bNotifyTick = true;

	DestinationKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UFlyToPoint, DestinationKey));
}



EBTNodeResult::Type UFlyToPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    
    AAIController* AICon = OwnerComp.GetAIOwner();
    if (!AICon) return EBTNodeResult::Failed;

    APawn* Pawn = AICon->GetPawn();
    if (!Pawn) return EBTNodeResult::Failed;


    //if (ACharacter* Char = Cast<ACharacter>(Pawn))
    //{
    //    if (Char->GetCharacterMovement()->MovementMode != MOVE_Flying)
    //    {
    //        Char->GetCharacterMovement()->SetMovementMode(MOVE_Flying);
    //    }
    //}

    return EBTNodeResult::InProgress;
}



void UFlyToPoint::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    AAIController* AICon = OwnerComp.GetAIOwner();
    if (!AICon) { FinishLatentTask(OwnerComp, EBTNodeResult::Failed); return; }

    APawn* Pawn = AICon->GetPawn();
    if (!Pawn) { FinishLatentTask(OwnerComp, EBTNodeResult::Failed); return; }

    ADroneAi* Drone = Cast<ADroneAi>(Pawn);
    if (!Drone) { FinishLatentTask(OwnerComp, EBTNodeResult::Failed); return; }

    const FVector Dest = OwnerComp.GetBlackboardComponent()->GetValueAsVector(DestinationKey.SelectedKeyName);
    if (Dest.IsNearlyZero())
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
        return;
    }
   // const FVector Loc = Pawn->GetActorLocation();
    const FVector To = Dest - Pawn->GetActorLocation();
    const float   Dist = To.Length();

    if (Dist <= AcceptanceRadius)
    {
       

        UWorld* World = Pawn->GetWorld();
        if (!World)
        {
            FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
            return;
        }

        FVector Start = Pawn->GetActorLocation();
        FVector End = Start - FVector(0.f, 0.f, 1000.f); 

        FHitResult HitResult;
        FCollisionQueryParams Params;
        Params.AddIgnoredActor(Pawn); 

        bool bHit = World->LineTraceSingleByChannel(
            HitResult,
            Start,
            End,
            ECC_Visibility,
            Params
        );

        Drone->Storage=Drone->Storage-1;
        if (Replenish) {
            Drone->Storage = 2;
        }
        if (bHit)
        {
            AActor* HitActor = HitResult.GetActor();
            if (HitActor)
            {
                ATree* Tree = Cast<ATree>(HitActor);
                if (Tree)
                {
                    int64 RowId = Drone->RowId;
                    int32 score = Drone->TreesExtinguished;
                    auto* DB = Drone->GetGameInstance()->GetSubsystem<UDatabaseSubsystem>();
                    score++;
                    DB->UpdateUserScore(RowId, score);

                    Tree->bIsOnFire = false;
                    Tree->bHasDroneAssigned = false;
                    Tree->ReleaseClaim();
                    //OwnerComp.GetBlackboardComponent()->SetValueAsObject("TreeActor", Tree);
                }
            }
        }


        DrawDebugLine(World, Start, End, FColor::Green, false, 2.f, 0, 2.f);
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
        return;
    }

  
    const FVector Dir = To.GetSafeNormal();
    const float InputScale = FMath::Clamp(MaxSteerAccel, 0.f, 5.f);

    Pawn->AddMovementInput(Dir, InputScale);

  
    if (ACharacter* Char = Cast<ACharacter>(Pawn))
    {
        const FVector Vel = Char->GetVelocity();
        if (!Vel.IsNearlyZero())
        {
            const FRotator Face = Vel.Rotation();
            Char->SetActorRotation(FRotator(0.f, Face.Yaw, 0.f));
        }
    }
}