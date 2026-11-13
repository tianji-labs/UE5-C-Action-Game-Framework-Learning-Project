// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstance/TianjiAnimInstance.h"
//角色
#include"Character/Tianji.h"
//数学库（计算）
#include "Kismet/KismetMathLibrary.h"
//移动组件
#include "GameFramework/CharacterMovementComponent.h"
//combat组件
#include "Component/Combat.h"

void UTianjiAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	MyCharacter = Cast<ATianji>(TryGetPawnOwner());
	if (MyCharacter)
	{
		MyCharacterMovement = MyCharacter->GetCharacterMovement();
		Combat = MyCharacter->FindComponentByClass<UCombat>();
	}

}

void UTianjiAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);
	if (MyCharacterMovement)
	{
		Velocity = MyCharacterMovement->Velocity;
		GroundSpeed = UKismetMathLibrary::VSizeXY(Velocity);
		IsFalling = MyCharacterMovement->IsFalling();
		WeaponState = MyCharacter->GetWeaponState();
	}
	if (!Combat.IsValid()) return;
	DeathPose = Combat->GetDeathPose();
}
