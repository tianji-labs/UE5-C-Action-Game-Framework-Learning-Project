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
	if(Enemy->GetCharacterMovement())
	{
		Enemy->GetCharacterMovement()->bOrientRotationToMovement = true;
	}
	if(Enemy)
	{
		EnemyControllerComponent = Enemy->FindComponentByClass<UEnemyControllerComponent>();
	}
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
	if (!IsValid(Enemy) || !Enemy->GetCharacterMovement()) return;
	Enemy->GetCharacterMovement()->MaxWalkSpeed = 200.f;
	PatrolTarget = PatrolPoints[FMath::RandRange(0, PatrolPoints.Num() - 1)];
	//随机移动到一个巡逻点
	MoveToLocation(PatrolTarget->GetActorLocation(), 100.f, true, true, true, false, 0, true);
	//MoveToTarget(PatrolTarget, MaxWalkSpeed);
}

//这次AI移动请求结束后会调用这个函数
void AEnemyAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	Super::OnMoveCompleted(RequestID, Result);

	if (IsValid(Enemy) && Enemy->GetEnemyState() == EEnemyState::EES_Patrolling)
	{
		//延迟2秒后再次开始巡逻
		GetWorldTimerManager().SetTimer(PatrolHandle, this, &AEnemyAIController::StartPatrolling, 2.0f, false);
	}
}

void AEnemyAIController::MoveToTarget(AActor* Target, float GroundSpeed)
{
	if (!IsValid(Target)) return;
	if (!IsValid(Enemy)||!Enemy->GetCharacterMovement()) return;
	Enemy->GetCharacterMovement()->MaxWalkSpeed = GroundSpeed;
	//创建移动请求
	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalActor(Target);
	MoveRequest.SetAcceptanceRadius(100.f);
	//移动到目标
	MoveTo(MoveRequest);

}

void AEnemyAIController::StartChaseTarget(AActor* Target)
{
	if (!IsValid(Enemy)) return;
	Enemy->SetEnemyState(EEnemyState::EES_Chasing);
	MaxWalkSpeed = 400.f;
	//持续追击
	FTimerDelegate MoveDel = FTimerDelegate::CreateUObject(this, &AEnemyAIController::MoveToTarget, Target, MaxWalkSpeed);
	GetWorldTimerManager().SetTimer(ChaseTimer, MoveDel, 0.25f, true);
}

void AEnemyAIController::EndChaseTarget()
{
	//结束追击
	GetWorldTimerManager().ClearTimer(ChaseTimer);

}