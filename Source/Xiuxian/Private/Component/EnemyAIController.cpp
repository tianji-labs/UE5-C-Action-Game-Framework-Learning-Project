// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/EnemyAIController.h"

#include "Component/EnemyControllerComponent.h"
//Enemy
#include"Enemy/Enemy.h"
//移动
#include "GameFramework/CharacterMovementComponent.h"
#include"navigation/PathFollowingComponent.h"


void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();

	Enemy = Cast<AEnemy>(GetPawn());
	Enemy->GetCharacterMovement()->bOrientRotationToMovement = true;

	EnemyControllerComponent = Enemy->FindComponentByClass<UEnemyControllerComponent>();
}

void AEnemyAIController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	GetWorldTimerManager().ClearTimer(PatrolHandle);
	Enemy->GetWorldTimerManager().ClearTimer(ChaseTimer);
}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	Enemy = Cast<AEnemy>(InPawn);
	if (IsValid(Enemy))
	{
		EnemyControllerComponent = Enemy->FindComponentByClass<UEnemyControllerComponent>();
	}
	if(IsValid(EnemyControllerComponent))
	{
		EnemyControllerComponent->InitializeAI(this);
	}
}

void AEnemyAIController::StartPatrolling()
{
	TArray<TObjectPtr<AActor>> PatrolPoints = EnemyControllerComponent->GetPatrolPoints();
	if (PatrolPoints.Num() == 0 || !IsValid(Enemy)) return;
	Enemy->SetEnemyState(EEnemyState::EES_Patrolling);
	Enemy->GetCharacterMovement()->MaxWalkSpeed = 200.f;
	PatrolTarget = PatrolPoints[FMath::RandRange(0, PatrolPoints.Num() - 1)];
	MoveToLocation(PatrolTarget->GetActorLocation());
}

//这次AI移动请求结束后会调用这个函数
void AEnemyAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	Super::OnMoveCompleted(RequestID, Result);

	if (IsValid(Enemy) && Enemy->GetEnemyState() == EEnemyState::EES_Patrolling)
	{
		GetWorldTimerManager().SetTimer(PatrolHandle, this, &AEnemyAIController::StartPatrolling, 2.0f, false);
	}
}

void AEnemyAIController::MoveToTarget(AActor* Target)
{
	if (!IsValid(Target)) return;
	Enemy->GetCharacterMovement()->MaxWalkSpeed = 400.f;
	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalActor(Target);
	MoveRequest.SetAcceptanceRadius(50.f);
	MoveTo(MoveRequest);

}

void AEnemyAIController::StartChaseTarget(AActor* Target)
{
	if (!IsValid(Enemy)) return;
	Enemy->SetEnemyState(EEnemyState::EES_Chasing);
	FTimerDelegate MoveDel = FTimerDelegate::CreateUObject(this, &AEnemyAIController::MoveToTarget, Target);
	Enemy->GetWorldTimerManager().SetTimer(ChaseTimer, MoveDel, 0.25f, true);
}

void AEnemyAIController::EndChaseTarget()
{
	if (!IsValid(Enemy) || !IsValid(Enemy->GetCharacterMovement())) return;
	Enemy->SetEnemyState(EEnemyState::EES_Patrolling);
	Enemy->GetCharacterMovement()->MaxWalkSpeed = 200.f;
	Enemy->GetWorldTimerManager().ClearTimer(ChaseTimer);

}