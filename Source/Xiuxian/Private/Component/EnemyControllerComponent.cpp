// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/EnemyControllerComponent.h"
#include "Component/EnemyAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Component/EnemyPerception.h"
#include "Component/AttributeComponent.h"
#include "Components/SphereComponent.h"
#include "Enemy/Enemy.h"


UEnemyControllerComponent::UEnemyControllerComponent()
{

	PrimaryComponentTick.bCanEverTick = false;

}

void UEnemyControllerComponent::BeginPlay()
{
	Super::BeginPlay();

	Owner = GetOwner();

	if (!IsValid(Owner)) return;
	EnemyPerception = Owner->GetComponentByClass<UEnemyPerception>();
	AttributeComponent = Owner->GetComponentByClass<UAttributeComponent>();
	AttackRangeSphere = Owner->FindComponentByClass<USphereComponent>();

	if (IsValid(EnemyPerception))
	{
		EnemyPerception->PerceptionUpdated.AddUObject(this, &UEnemyControllerComponent::ReceivePerceptionUpdated);
	}
	if (AttackRangeSphere.IsValid())
	{
		AttackRangeSphere->OnComponentBeginOverlap.AddDynamic(this, &UEnemyControllerComponent::OnAttackRangeBeginOverlap);
		AttackRangeSphere->OnComponentEndOverlap.AddDynamic(this, &UEnemyControllerComponent::OnAttackRangeEndOverlap);
	}
}

void UEnemyControllerComponent::ReceivePerceptionUpdated(AActor* FoundActor,bool bSensed)
{
	if (!IsValid(EnemyAIController)) return;
	if (bSensed)
	{
		ChaseTarget = FoundActor;
		UpdateBehaviorState();
	}
	else
	{
		ChaseTarget = nullptr;
		UpdateBehaviorState();
	}
}

void UEnemyControllerComponent::OnAttackRangeBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IsValid(AttributeComponent) || AttributeComponent->IsDead()) return;

	AttackTarget = OtherActor;

	UpdateBehaviorState();
}

void UEnemyControllerComponent::OnAttackRangeEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!IsValid(AttributeComponent) || AttributeComponent->IsDead()) return;

	AttackTarget = nullptr;

	UpdateBehaviorState();
}

void UEnemyControllerComponent::InitializeAI(AEnemyAIController* InEnemyAIController)
{
	EnemyAIController = InEnemyAIController;
	//确保AttributeComponent有效并且更新行为状态
	if (!IsValid(AttributeComponent))
	{
		AttributeComponent = GetOwner()->FindComponentByClass<UAttributeComponent>();
	}

	UpdateBehaviorState();
}

/* 有限状态机 */
void UEnemyControllerComponent::UpdateBehaviorState()
{
	if (!IsValid(AttributeComponent)) return;
	if (AttributeComponent->IsDead())
	{
		ChangeEnemyState(EEnemyState::EES_Dead);
	}
	else if(IsValid(AttackTarget))
	{
		ChangeEnemyState(EEnemyState::EES_Attacking);
	}
	else if(IsValid(ChaseTarget))
	{
		ChangeEnemyState(EEnemyState::EES_Chasing);
	}
	else
	{
		ChangeEnemyState(EEnemyState::EES_Patrolling);
	}
}

void UEnemyControllerComponent::ChangeEnemyState(EEnemyState NewState)
{
	EEnemyState EnemyState = AttributeComponent->GetEnemyState();

	if (EnemyState == NewState) return;

	if (!CanChangeState(NewState)) return;
	
	if (!IsValid(AttributeComponent)) return;

	ExitState(AttributeComponent->GetEnemyState());
	AttributeComponent->ChangeBehaviorState(NewState);
	EnterState(NewState);
}
//判断是否允许改变状态
bool UEnemyControllerComponent::CanChangeState(EEnemyState NewState)
{
	EEnemyState EnemyState = AttributeComponent->GetEnemyState();
	if (EnemyState == EEnemyState::EES_Dead)
	{
		return false;
	}

	if (NewState == EEnemyState::EES_Dead)
	{
		return true;
	}

	bool bCanChange = false;
	switch (EnemyState)
	{
	case EEnemyState::EES_NoState:
	
		return true;
	
	case EEnemyState::EES_Patrolling:
	
		bCanChange = NewState == EEnemyState::EES_Chasing || NewState == EEnemyState::EES_Attacking;
		return bCanChange;
	
	case EEnemyState::EES_Chasing:
	
		bCanChange = NewState == EEnemyState::EES_Attacking || NewState == EEnemyState::EES_Patrolling;
		return bCanChange;
	
	case EEnemyState::EES_Attacking:
	
		bCanChange = NewState == EEnemyState::EES_Chasing || NewState == EEnemyState::EES_Patrolling;
		return bCanChange;
	default:
	
		return false;
	
	}

}
//清除旧状态
void UEnemyControllerComponent::ExitState(EEnemyState OldState)
{
	OldState = AttributeComponent->GetEnemyState();
	switch (OldState)
	{
	case EEnemyState::EES_NoState:
	{
		return;
	}
	case EEnemyState::EES_Patrolling:
	{
		return;
	}
	case EEnemyState::EES_Chasing:
	{
		if (!IsValid(EnemyAIController)) return;
		EnemyAIController->EndChaseTarget();
		return;
	}
	case EEnemyState::EES_Attacking:
	{
		if (!IsValid(Owner)) return;
		AEnemy* Enemy = Cast<AEnemy>(Owner);
		if (!IsValid(Enemy)) return;	
		Enemy->StopAttack();
		return;
	}
	default:
		return;
	}
}
//初始化新状态
void UEnemyControllerComponent::EnterState(EEnemyState NewState)
{
	switch (NewState)
	{
	case EEnemyState::EES_NoState:
	{
		return;
	}
	case EEnemyState::EES_Patrolling:
	{
		if (!IsValid(EnemyAIController)) return;
		EnemyAIController->StartPatrolling();
		return;
	}
	case EEnemyState::EES_Chasing:
	{
		if (!IsValid(EnemyAIController)) return;
		EnemyAIController->StartChaseTarget(ChaseTarget);
		return;
	}
	case EEnemyState::EES_Attacking:
	{
		if (!IsValid(Owner)) return;
		AEnemy* Enemy = Cast<AEnemy>(Owner);
		if (!IsValid(Enemy)) return;
		Enemy->Attack(AttackTarget);
		return;
	}
	default:
		return;
	}
}



