// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/EnemyAIController.h"

#include "Component/EnemyControllerComponent.h"
#include"Component/AttributeComponent.h"
//Enemy
#include"Enemy/Enemy.h"
//移动
#include "GameFramework/CharacterMovementComponent.h"
#include"navigation/PathFollowingComponent.h"


void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();
}

void AEnemyAIController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	GetWorldTimerManager().ClearTimer(PatrolHandle);
	GetWorldTimerManager().ClearTimer(ChaseTimer);
}


//控制器接管时调用
void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	Enemy = Cast<AEnemy>(InPawn);

	if(!IsValid(Enemy)) return;
	EnemyControllerComponent = Enemy->FindComponentByClass<UEnemyControllerComponent>();
	AttributeComponent = Enemy->FindComponentByClass<UAttributeComponent>();

	if(!IsValid(Enemy->GetCharacterMovement())) return;
	Enemy->GetCharacterMovement()->bOrientRotationToMovement = true;

	if (!IsValid(EnemyControllerComponent)) return;
	EnemyControllerComponent->InitializeAI(this);
}


/* 巡逻 */
void AEnemyAIController::StartPatrolling()
{
	if (!IsValid(Enemy) || !Enemy->GetCharacterMovement()) return;
	Enemy->GetCharacterMovement()->MaxWalkSpeed = 200.f;

	//随机选点
	if(!IsValid(EnemyControllerComponent)) return;
	PatrolTarget = ChoosePatrolTarget(EnemyControllerComponent->GetPatrolPoints());

	//随机移动到一个巡逻点
	if (!IsValid(PatrolTarget)) return;
	MoveToLocation(PatrolTarget->GetActorLocation(), 100.f, true, true, true, false, 0, true);
}

void AEnemyAIController::EndPatrolling()
{
	GetWorldTimerManager().ClearTimer(PatrolHandle);
}


TObjectPtr<AActor> AEnemyAIController::ChoosePatrolTarget(const TArray<TObjectPtr<AActor>>& PatrolPoints)
{
	TArray<TObjectPtr<AActor>> RemainingPatrolPoints;

	for (TObjectPtr<AActor> PatrolPoint : PatrolPoints)
	{
		if (PatrolPoint != PatrolTarget)
		{
			RemainingPatrolPoints.AddUnique(PatrolPoint);
		}
	}
	if (RemainingPatrolPoints.Num() > 0)
	{
		int32 Index = FMath::RandRange(0, RemainingPatrolPoints.Num() - 1);
		return RemainingPatrolPoints[Index];
	}
	return nullptr;
}
//这次AI移动请求结束后会调用这个函数
void AEnemyAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	Super::OnMoveCompleted(RequestID, Result);

	if (IsValid(AttributeComponent) && AttributeComponent->GetEnemyState() == EEnemyState::EES_Patrolling)
	{
		//延迟2秒后再次开始巡逻
		GetWorldTimerManager().SetTimer(PatrolHandle, this, &AEnemyAIController::StartPatrolling, 2.0f, false);
	}
}

/* 追击 */
void AEnemyAIController::StartChaseTarget(AActor* Target)
{
	if (!IsValid(Enemy)) return;
	MaxWalkSpeed = 400.f;
	//持续追击
	FTimerDelegate MoveDel = FTimerDelegate::CreateUObject(this, &AEnemyAIController::MoveToTarget, Target, MaxWalkSpeed);
	GetWorldTimerManager().SetTimer(ChaseTimer, MoveDel, 0.25f, true);
}

void AEnemyAIController::MoveToTarget(AActor* Target, float GroundSpeed)
{
	if (!IsValid(Target)) return;
	if (!IsValid(Enemy)||!Enemy->GetCharacterMovement()) return;
	Enemy->GetCharacterMovement()->MaxWalkSpeed = GroundSpeed;
	//创建移动请求
	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalActor(Target);
	MoveRequest.SetAcceptanceRadius(130.f);
	//移动到目标
	MoveTo(MoveRequest);

}

void AEnemyAIController::EndChaseTarget()
{
	//结束追击
	GetWorldTimerManager().ClearTimer(ChaseTimer);

}
