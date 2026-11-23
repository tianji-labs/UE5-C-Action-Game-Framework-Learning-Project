// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/AI/Task/BTTask_DeathHandle.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Component/EnemyAIController.h"
#include "Enemy/Enemy.h"

EBTNodeResult::Type UBTTask_DeathHandle::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (!IsValid(OwnerComp.GetAIOwner())) return EBTNodeResult::Failed;
	AEnemy* Enemy = Cast<AEnemy>(OwnerComp.GetAIOwner()->GetPawn());
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();

	if (!IsValid(Enemy) || !IsValid(Blackboard)) return EBTNodeResult::Failed;
	AActor* TargetActor = Cast<AActor>(Blackboard->GetValueAsObject(TargetActorKey.SelectedKeyName));

	//播放死亡动画
	Enemy->PlayDeathMontage(TargetActor);
	//死亡处理
	Enemy->Die();

	return EBTNodeResult::Succeeded;
}
