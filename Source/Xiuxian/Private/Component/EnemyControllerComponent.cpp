// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/EnemyControllerComponent.h"
#include"Component/EnemyAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include"Component/EnemyPerception.h"


UEnemyControllerComponent::UEnemyControllerComponent()
{

	PrimaryComponentTick.bCanEverTick = false;

}

void UEnemyControllerComponent::BeginPlay()
{
	Super::BeginPlay();

	EnemyPerception = GetOwner()->GetComponentByClass<UEnemyPerception>();

	if (IsValid(EnemyPerception))
	{
		EnemyPerception->PerceptionUpdated.AddUObject(this, &UEnemyControllerComponent::ReceivePerceptionUpdated);
	}
}

void UEnemyControllerComponent::ReceivePerceptionUpdated(AActor* FoundActor,bool bSensed)
{
	if (!IsValid(EnemyAIController)) return;
	if (bSensed)
	{
		EnemyAIController->StartChaseTarget(FoundActor);
	}
	else
	{
		EnemyAIController->EndChaseTarget();
	}
}
void UEnemyControllerComponent::InitializeAI(AEnemyAIController* InEnemyAIController)
{
	EnemyAIController = InEnemyAIController;
	if (!IsValid(EnemyAIController)) return;
	EnemyAIController->StartPatrolling();
}



