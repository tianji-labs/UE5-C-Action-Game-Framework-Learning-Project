// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/EnemyControllerComponent.h"
#include"Component/EnemyAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include"Component/EnemyPerception.h"
#include "Component/AttributeComponent.h"


UEnemyControllerComponent::UEnemyControllerComponent()
{

	PrimaryComponentTick.bCanEverTick = false;

}

void UEnemyControllerComponent::BeginPlay()
{
	Super::BeginPlay();

	AActor* Owner = GetOwner();
	if (!IsValid(Owner)) return;
	EnemyPerception = Owner->GetComponentByClass<UEnemyPerception>();
	AttributeComponent = Owner->GetComponentByClass<UAttributeComponent>();

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
		ChaseTarget = FoundActor;
		UpdateBehaviorState();
		EnemyAIController->StartChaseTarget(ChaseTarget);
	}
	else
	{
		ChaseTarget = nullptr;
		UpdateBehaviorState();
		EnemyAIController->EndChaseTarget();
		EnemyAIController->StartPatrolling();
	}
}

void UEnemyControllerComponent::InitializeAI(AEnemyAIController* InEnemyAIController)
{
	EnemyAIController = InEnemyAIController;
	//确保AttributeComponent有效并且更新行为状态
	if (!IsValid(AttributeComponent))
	{
		AttributeComponent = GetOwner()->FindComponentByClass<UAttributeComponent>();
	}
	if (!IsValid(AttributeComponent)) return;
	UpdateBehaviorState();
	//确保EnemyAIController有效并且处于巡逻状态然后开始巡逻
	if (!IsValid(EnemyAIController) && AttributeComponent->GetEnemyState() != EEnemyState::EES_Patrolling) return;
	EnemyAIController->StartPatrolling();
}

void UEnemyControllerComponent::UpdateBehaviorState()
{
	if (!IsValid(AttributeComponent)) return;
	if (AttributeComponent->IsDead())
	{
		AttributeComponent->ChangeBehaviorState(EEnemyState::EES_Dead);
	}
	else if(IsValid(ChaseTarget))
	{
		AttributeComponent->ChangeBehaviorState(EEnemyState::EES_Chasing);
	}
	else
	{
		AttributeComponent->ChangeBehaviorState(EEnemyState::EES_Patrolling);
	}
}



