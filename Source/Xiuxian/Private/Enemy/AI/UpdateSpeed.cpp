// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/AI/UpdateSpeed.h"
#include "BehaviorTree/BehaviorTreeComponent.h"

#include "Component/EnemyAIController.h"
#include "Enemy/Enemy.h"
#include "GameFramework/CharacterMovementComponent.h"

UUpdateSpeed::UUpdateSpeed()
{
	bNotifyTick = false;
	bNotifyBecomeRelevant = true;
}

//服务激活时修改速度
void UUpdateSpeed::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (!IsValid(OwnerComp.GetAIOwner())) return;
	AEnemy* Enemy = Cast<AEnemy>(OwnerComp.GetAIOwner()->GetPawn());

	if (!IsValid(Enemy)) return;
	Enemy->UpdateSpeed(NewSpeed);
}
