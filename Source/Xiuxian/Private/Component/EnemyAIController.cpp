// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/EnemyAIController.h"


//接管后运行行为树
void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	RunBehaviorTree(BehaviorTreeAsset);
}
