// Fill out your copyright notice in the Description page of Project Settings.


#include "GetAirLocation.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Tree.h"
#include "Manager.h"

UGetAirLocation::UGetAirLocation()
{
	NodeName = TEXT("Pick Random Air Location");
	LocationKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UGetAirLocation, LocationKey));
    
}

EBTNodeResult::Type UGetAirLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    
        AAIController* AICon = OwnerComp.GetAIOwner();
        if (!AICon) return EBTNodeResult::Failed;

        APawn* Pawn = AICon->GetPawn();
        if (!Pawn) return EBTNodeResult::Failed;

        UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();

        if (!landing) {

            ClosestTree = nullptr;

            // Gather trees
            TArray<AActor*> FoundActors;
            UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATree::StaticClass(), FoundActors);

            TArray<ATree*> FoundTrees;
            FoundTrees.Reserve(FoundActors.Num());
            for (AActor* Actor : FoundActors)
            {
                if (ATree* T = Cast<ATree>(Actor))
                {

                    if (T->bIsOnFire)
                    {
                        FoundTrees.Add(T);
                    }
                }
            }

            FoundTrees.Sort([Pawn](const ATree& A, const ATree& B)
                {
                    return FVector::DistSquared(A.GetActorLocation(), Pawn->GetActorLocation()) <
                        FVector::DistSquared(B.GetActorLocation(), Pawn->GetActorLocation());
                });

            for (ATree* Tree : FoundTrees)
            {
                if (!IsValid(Tree)) continue;

                APawn* Claimant = Tree->GetClaim();

                if (IsValid(Claimant) && Claimant != Pawn)
                {
                    UE_LOG(LogTemp, Warning, TEXT("DRONE: %s sees %s occupied by %s"),
                        *Pawn->GetName(), *Tree->GetName(), *Claimant->GetName());
                    continue;
                }

                if (Tree->TryClaim(Pawn))
                {
                    UE_LOG(LogTemp, Warning, TEXT("DRONE: %s claiming target %s"),
                        *Pawn->GetName(), *Tree->GetName());

                    ClosestTree = Tree;
                    break;
                }
                else
                {

                    continue;
                }
            }

            if (ClosestTree)
            {
                const FVector Target = ClosestTree->GetActorLocation() + FVector(0.f, 0.f, 1000.f);
                BB->SetValueAsVector(LocationKey.SelectedKeyName, Target);
                return EBTNodeResult::Succeeded;
            }

            BB->ClearValue(LocationKey.SelectedKeyName);
            AICon->StopMovement();
            return EBTNodeResult::Failed;
        }
        else {
            UE_LOG(LogTemp, Display, TEXT("DRONE : REQUESTING MANGER"));
            AManager* Manager = nullptr;

            TArray<AActor*> FoundActors;
            UGameplayStatics::GetAllActorsOfClass(GetWorld(), AManager::StaticClass(), FoundActors);

            TArray<AManager*> FoundManagers;
            FoundManagers.Reserve(FoundActors.Num());

            for (AActor* Actor : FoundActors)
            {
                if (AManager* T = Cast<AManager>(Actor))
                {

                    
                    FoundManagers.Add(T);
                    
                }
            }

            FoundManagers.Sort([Pawn](const AManager& A, const AManager& B)
                {
                    return FVector::DistSquared(A.GetActorLocation(), Pawn->GetActorLocation()) <
                        FVector::DistSquared(B.GetActorLocation(), Pawn->GetActorLocation());
                });
            for (AManager* Man : FoundManagers) {
                    UE_LOG(LogTemp, Warning, TEXT("Found MANAGER"));
                if (Man) {
                    Manager = Man;
                    break;
                }
            }
            if (Manager)
            {
                const FVector Target = Manager->GetActorLocation() + FVector(0.f, 0.f, 10);
                BB->SetValueAsVector(LocationKey.SelectedKeyName, Target);
                return EBTNodeResult::Succeeded;
            }

            BB->ClearValue(LocationKey.SelectedKeyName);
            AICon->StopMovement();
            return EBTNodeResult::Failed;
        }
}

