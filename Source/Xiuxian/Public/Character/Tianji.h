// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Tianji.generated.h"
//定义委托
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnOpenMenuRequest);

//移动
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
//人物旋转
class UCharacterMovementComponent;
//第三人称视角
class USpringArmComponent;
class UCameraComponent;
//武器
class AWeapon;


UCLASS()
class XIUXIAN_API ATianji : public ACharacter
{
	GENERATED_BODY()

public:
	ATianji();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//公开委托，外部可以绑定监听
	UPROPERTY(BlueprintAssignable, Category = "PersonalAsset|Menu")
	FOnOpenMenuRequest OnOpenMenuRequest;


	AWeapon* OverlappingItem;
	//UE的FORCEINLINE强制内联
	FORCEINLINE void SetOverlappingItem(AWeapon* Item) { OverlappingItem = Item; }

protected:
	virtual void BeginPlay() override;


private:
/*人物旋转*/
	UCharacterMovementComponent* MovementComponent;
/*第三人称视角*/
	void Look(const FInputActionValue& Value);

	UPROPERTY(VisibleAnywhere, Category = "PersonalAsset|Camera")
	USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere, Category = "PersonalAsset|Camera")
	UCameraComponent* Camera;
	UPROPERTY(EditDefaultsOnly,Category = "PersonalAsset|Camera")
	UInputAction* IA_Look;

/* 移动 */
	void Move(const FInputActionValue& Value);
	void Release(const FInputActionValue& Value);
	void Sprint();
	void Jump();


	//UCharacterMovementComponent* MovementComponent;

	UPROPERTY(EditDefaultsOnly,Category = "PersonalAsset|Input")
	UInputMappingContext* IMC_Tianji;
	UPROPERTY(EditDefaultsOnly,Category = "PersonalAsset|Input")
	UInputAction* IA_Move;
	UPROPERTY(EditDefaultsOnly,Category = "PersonalAsset|Input")
	UInputAction* IA_Sprint;
	UPROPERTY(EditDefaultsOnly,Category = "PersonalAsset|Input")
	UInputAction* IA_Jump;

/* 菜单 */
	void Menu();

	UPROPERTY(EditDefaultsOnly,Category = "PersonalAsset|Input")
	UInputAction* IA_Menu;
/* 拾取 */
	UPROPERTY(EditDefaultsOnly, Category = "PersonalAsset|Input")
	UInputAction* IA_Pickup;


	void Pickup();
	void PickupWeapon(AWeapon* Weapon);
};
