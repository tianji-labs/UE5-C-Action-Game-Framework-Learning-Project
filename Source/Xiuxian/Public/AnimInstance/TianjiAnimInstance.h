// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include"Character/StateType.h"
#include "TianjiAnimInstance.generated.h"


class ATianji;
class UCharacterMovementComponent;

UCLASS()
class XIUXIAN_API UTianjiAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
protected:
	//初始化重写
	virtual void NativeInitializeAnimation() override;
	//更新重写
	virtual void NativeUpdateAnimation(float DeltaTime) override;

	//角色
	UPROPERTY(BlueprintReadOnly)
	ATianji* MyCharacter; 
	//移动组件
	UPROPERTY(BlueprintReadOnly, Category = "PersonalAsset|Movement")
	UCharacterMovementComponent* MyCharacterMovement;  
	//是否在空中
	UPROPERTY(BlueprintReadOnly, Category = "PersonalAsset|Movement")
	bool IsFalling;
	//向量
	UPROPERTY(BlueprintReadOnly, Category = "PersonalAsset|Movement")
	FVector Velocity;
	//地面速度
	UPROPERTY(BlueprintReadOnly, Category = "PersonalAsset|Movement")
	float GroundSpeed;
	UPROPERTY(BlueprintReadOnly, Category = "PersonalAsset|StateType")
	EWeaponState WeaponState;
};
