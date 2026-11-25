// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/EnemyAttributeComponent.h"

#include "BehaviorTree/BlackboardComponent.h"

#include "Enemy/Enemy.h"


void UEnemyAttributeComponent::HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{

	if (IsDead()) return;
	//扣除生命值
	CurrentHealth = FMath::Clamp(CurrentHealth - Damage, 0.f, MaxHealth);

	//若生命值归零，设置黑板值IsDead
	if (IsDead())
	{
		AEnemy* Enemy = Cast<AEnemy>(GetOwner());
		
		if (IsValid(Enemy) && IsValid(Enemy->GetBlackboardComponent()))
		{
			Enemy->GetBlackboardComponent()->SetValueAsBool(FName("IsDead"), true);
		}
	}

	OnEnemyHealthChanged.Broadcast(GetHealthBarPercent());
}
