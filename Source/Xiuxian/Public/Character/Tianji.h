// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
//状态类型
#include "StateType.h"
#include "Interface/GetHitInterface.h"
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
//combat组件
class UCombat;
//属性
class UAttributeComponent;


UCLASS()
class XIUXIAN_API ATianji : public ACharacter, public IGetHitInterface
{
	GENERATED_BODY()

public:
	ATianji();

	virtual void Tick(float DeltaTime) override;



protected:
	virtual void BeginPlay() override;

private:
/*人物旋转*/
	UPROPERTY()
	TObjectPtr<UCharacterMovementComponent> MovementComponent;
/*第三人称视角*/
	void Look(const FInputActionValue& Value);

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	TObjectPtr<USpringArmComponent> SpringArm;
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	TObjectPtr<UCameraComponent> Camera;
	UPROPERTY(EditDefaultsOnly,Category = "Camera")
	TObjectPtr<UInputAction> IA_Look;

/* 移动 */
	void Move(const FInputActionValue& Value);
	void Release(const FInputActionValue& Value);
	void Sprint();
	void Jump();

	UPROPERTY(EditDefaultsOnly,Category = "Input")
	TObjectPtr<UInputAction> IA_Move;
	UPROPERTY(EditDefaultsOnly,Category = "Input")
	TObjectPtr<UInputAction> IA_Sprint;
	UPROPERTY(EditDefaultsOnly,Category = "Input")
	TObjectPtr<UInputAction> IA_Jump;

/* 菜单 */
	void Menu();

	UPROPERTY(EditDefaultsOnly,Category = "Input")
	TObjectPtr<UInputAction> IA_Menu;

/* 拾取 */
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_Pickup;


	void Pickup();
	void PickupWeapon(AWeapon* Weapon);

/* 攻击 */
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_Attack;

	void Attack();

/* 受击 */
	void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter);
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

/* 状态 */
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TObjectPtr<UCombat> Combat;

	AWeapon* EquippedWeapon = nullptr;
	EWeaponState WeaponState = EWeaponState::EWS_Unequipped;
/* 属性 */
	UPROPERTY(EditDefaultsOnly, Category = "Attribute")
	TObjectPtr<UAttributeComponent> AttributeComponent; 


public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//公开委托，外部可以绑定监听
	UPROPERTY(BlueprintAssignable, Category = "Menu")
	FOnOpenMenuRequest OnOpenMenuRequest;
	UPROPERTY(VisibleAnywhere)
	AWeapon* OverlappingItem;


	//UE的FORCEINLINE强制内联
	FORCEINLINE void SetOverlappingItem(AWeapon* Item) { OverlappingItem = Item; }

	FORCEINLINE EWeaponState GetWeaponState() const { return WeaponState; }

	FORCEINLINE AWeapon* GetEquippedWeapon() const { return EquippedWeapon; }

	void PlayMontageSection(UAnimMontage* Montage, const FName& SectionName);

};
