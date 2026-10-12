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

	EnemyPerception = GetOwner()->GetComponentByClass<UEnemyPerception>();
	AttributeComponent = GetOwner()->GetComponentByClass<UAttributeComponent>();

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
	if (!IsValid(EnemyAIController)) return;
	UpdateBehaviorState();
	UE_LOG(LogTemp, Warning, TEXT("InitializeAI"));
	EnemyAIController->StartPatrolling();
}

void UEnemyControllerComponent::UpdateBehaviorState()
{
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



