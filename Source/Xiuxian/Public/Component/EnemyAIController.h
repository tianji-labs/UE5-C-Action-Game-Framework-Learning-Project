// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

struct FTimerHandle;
class AEnemy;
class UEnemyControllerComponent;
struct FTimerHandle;

UCLASS()
class XIUXIAN_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()
public:

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void OnPossess(APawn* InPawn) override;
private:
	UPROPERTY(VisibleAnywhere, Category = "Tool")
	float MaxWalkSpeed = 200.f;


	UPROPERTY()
	TObjectPtr<AEnemy> Enemy;


	UPROPERTY()
	TObjectPtr<UEnemyControllerComponent> EnemyControllerComponent;

	UPROPERTY()	
	TObjectPtr<AActor> PatrolTarget;


	FTimerHandle PatrolHandle;

	virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;
	//×·»÷
	FTimerHandle ChaseTimer;
public:


	void MoveToTarget(AActor* Target,float GroundSpeed);
	void StartPatrolling();

	//chasing
	void StartChaseTarget(AActor* Target);
	void EndChaseTarget();

};
