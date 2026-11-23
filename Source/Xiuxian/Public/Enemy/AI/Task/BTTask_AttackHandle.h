// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_AttackHandle.generated.h"

class UBehaviorTreeComponent;

UCLASS()
class XIUXIAN_API UBTTask_AttackHandle : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTask_AttackHandle();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UBehaviorTreeComponent* BehaviorTreeComp = nullptr;

	FOnMontageEnded OnAttackMontageEnded;

	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);
};
