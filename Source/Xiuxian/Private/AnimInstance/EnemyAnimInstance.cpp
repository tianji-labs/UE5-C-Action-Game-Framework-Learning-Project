// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstance/EnemyAnimInstance.h"
#include"GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

#include"Enemy/Enemy.h"


void UEnemyAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Enemy = Cast<AEnemy>(TryGetPawnOwner());

	if (!Enemy.IsValid())
	{
		return;
	}
	MyCharacterMovement = Enemy->FindComponentByClass<UCharacterMovementComponent>();

}

void UEnemyAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	//更新Enemy当前的死亡姿势、速度
	if (!Enemy.IsValid())
	{
		return;
	}
	DeathPose = Enemy->GetDeathPose();

	if (!MyCharacterMovement.IsValid())
	{
		return;
	}
	Velocity = MyCharacterMovement->Velocity;

	GroundSpeed = UKismetMathLibrary::VSizeXY(Velocity);
}
