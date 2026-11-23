// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/AI/Task/BTTask_DirectedSelectionOfPath.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Enemy/Enemy.h"
#include "Component/EnemyAIController.h"

EBTNodeResult::Type UBTTask_DirectedSelectionOfPath::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (!IsValid(OwnerComp.GetAIOwner())) return EBTNodeResult::Failed;
	AEnemy* Enemy = Cast<AEnemy>(OwnerComp.GetAIOwner()->GetPawn());
	UBlackboardComponent* Blackboard= OwnerComp.GetBlackboardComponent();

	//获取黑板与Enemy
	if (!IsValid(Enemy) || !IsValid(Blackboard)) return EBTNodeResult::Failed;
	FVector PatrolLocation = Blackboard->GetValueAsVector(FName("PatrolLocation"));
	TArray<TObjectPtr<AActor>> PatrolTargets = Enemy->GetPatrolTargets();

	//创建数组存放余下巡逻位置
	TArray<TObjectPtr<AActor>> NewPatrolTargets;
	if (PatrolTargets.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("PatrolTargets is nullptr"));
		return EBTNodeResult::Failed;
	}
	for (int Index = 0; Index < PatrolTargets.Num(); Index++)
	{
		if (PatrolTargets[Index].Get()->GetActorLocation() != PatrolLocation)
		{
			NewPatrolTargets.AddUnique(PatrolTargets[Index]);
		}
	}

	//获取巡逻点从新数组
	const int32 NewPatrolTargetsNum = NewPatrolTargets.Num();
	if (NewPatrolTargetsNum > 0)
	{
		const int32 TargetSelection = FMath::RandRange(0, NewPatrolTargetsNum - 1);
		FVector NewPatrolLocation = NewPatrolTargets[TargetSelection].Get()->GetActorLocation();
		Blackboard->SetValueAsVector(FName("PatrolLocation"), NewPatrolLocation);
	}

	return EBTNodeResult::Succeeded;
}
