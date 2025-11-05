// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tree.generated.h"

class UNiagaraComponent;

UCLASS()
class DRONEMANAGEMENTSYS_API ATree : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATree();

	UPROPERTY(EditAnywhere)
	bool bIsOnFire =false;

	


	UPROPERTY(EditAnywhere)
	bool bShouldBeOnFire = false;

	UPROPERTY(EditAnywhere)
	int SpreadRadius = 500;

	UPROPERTY(EditAnywhere)
	int SpreadSpeed = 2;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UNiagaraComponent* NiagaraComp;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	virtual void Tick(float DeltaTime) override;



	FTimerHandle TimerHandle_MyTimer;
	ATree* ClosestTree;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	APawn* ClaimedBy;

	UPROPERTY(EditAnywhere)
	bool bHasDroneAssigned = false;

	APawn* GetClaim();

	bool TryClaim(APawn* Claimant);

	void ReleaseClaim();

	bool IsClaimedBy(AActor* Claimant) const;


	bool TryAssignDrone();

	UFUNCTION()
	void OnTimerTick();


};
