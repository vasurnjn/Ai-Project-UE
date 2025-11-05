

#include "Tree.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "TimerManager.h"

ATree::ATree()
{

	PrimaryActorTick.bCanEverTick = true;
	NiagaraComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComp"));
	NiagaraComp->SetupAttachment(RootComponent);
	NiagaraComp->SetAutoActivate(false);
}

void ATree::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(
		TimerHandle_MyTimer,
		this,
		&ATree::OnTimerTick,
		SpreadSpeed,         
		true          
	);
	
}

// Called every frame
void ATree::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bIsOnFire && NiagaraComp && NiagaraComp->IsActive())
	{
		NiagaraComp->Deactivate();
	}

}

void ATree::OnTimerTick()
{
	if (bShouldBeOnFire) {
		NiagaraComp->Activate();
		bIsOnFire = true;
		bShouldBeOnFire = false;
	}

	if (bIsOnFire) {
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATree::StaticClass(), FoundActors);

		TArray<ATree*> FoundTrees;
		for (AActor* Actor : FoundActors)
		{
			ATree* Tree = Cast<ATree>(Actor);
			if (Tree)
			{
				FoundTrees.Add(Tree);
			}
		}

		FoundTrees.Sort([this](const ATree& A, const ATree& B)
			{
				return FVector::DistSquared(A.GetActorLocation(), GetActorLocation()) <
					FVector::DistSquared(B.GetActorLocation(), GetActorLocation());
			});


		for (ATree* Tree : FoundTrees)
		{
			if (Tree) {
				if (Tree->bIsOnFire == false)
				{
					ClosestTree = Tree;
					break;
				}
			}
		}
		if (ClosestTree)
		{
			const float Distance = FVector::Dist(ClosestTree->GetActorLocation(), GetActorLocation());

			if (Distance <= SpreadRadius) 
			{
				ClosestTree->bShouldBeOnFire = FMath::RandBool(); 
			}
		}

	}
}

APawn* ATree::GetClaim()
{
	return ClaimedBy;
}

bool ATree::TryClaim(APawn* Claimant)
{
	if (IsValid(ClaimedBy)) {
		UE_LOG(LogTemp, Warning, TEXT("TREE:ALREADY CLAIMED"));
		return false;
	}
	else {
		ClaimedBy = Claimant;
		UE_LOG(LogTemp, Warning, TEXT("TREE: DRONE ALLOCATED"));
		return true;
	}

}

void ATree::ReleaseClaim()
{

	ClaimedBy = nullptr;
	

}
//
//bool ATree::IsClaimedBy(AActor* Claimant) const
//{
//	return ClaimedBy.IsValid() && ClaimedBy.Get() == Claimant;
//}
//
//bool ATree::TryAssignDrone()
//{
//	if (bIsOnFire && !bHasDroneAssigned)
//	{
//		bHasDroneAssigned = true;
//		return true;
//	}
//	return false;
//}
