// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

class UBehaviorTree;

UCLASS()
class XIUXIAN_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()
protected:
	virtual void OnPossess(APawn* InPawn) override;

private:
	TObjectPtr<UBehaviorTree> BehaviorTreeAsset;
	
};
