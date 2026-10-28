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


UCLASS()
class XIUXIAN_API ATianji : public ACharacter, public IGetHitInterface
{
	GENERATED_BODY()

public:
	ATianji();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//公开委托，外部可以绑定监听
	UPROPERTY(BlueprintAssignable, Category = "PersonalAsset|Menu")
	FOnOpenMenuRequest OnOpenMenuRequest;
	UPROPERTY(VisibleAnywhere)
	AWeapon* OverlappingItem;


	//UE的FORCEINLINE强制内联
	FORCEINLINE void SetOverlappingItem(AWeapon* Item) { OverlappingItem = Item; }
/*状态*/
	UFUNCTION(BlueprintCallable)
	void SetActionState(EActionState NewState);
	FORCEINLINE const EActionState& GetActionState() const { return ActionState; }
	FORCEINLINE const EWeaponState& GetWeaponState() const { return WeaponState; }

protected:
	virtual void BeginPlay() override;

private:
/*人物旋转*/
	UPROPERTY()
	TObjectPtr<UCharacterMovementComponent> MovementComponent;
/*第三人称视角*/
	void Look(const FInputActionValue& Value);

	UPROPERTY(VisibleAnywhere, Category = "Person|Camera")
	TObjectPtr<USpringArmComponent> SpringArm;
	UPROPERTY(VisibleAnywhere, Category = "Person|Camera")
	TObjectPtr<UCameraComponent> Camera;
	UPROPERTY(EditDefaultsOnly,Category = "Person|Camera")
	TObjectPtr<UInputAction> IA_Look;

/* 移动 */
	void Move(const FInputActionValue& Value);
	void Release(const FInputActionValue& Value);
	void Sprint();
	void Jump();

	UPROPERTY(EditDefaultsOnly,Category = "Person|Input")
	TObjectPtr<UInputAction> IA_Move;
	UPROPERTY(EditDefaultsOnly,Category = "Person|Input")
	TObjectPtr<UInputAction> IA_Sprint;
	UPROPERTY(EditDefaultsOnly,Category = "Person|Input")
	TObjectPtr<UInputAction> IA_Jump;

/* 菜单 */
	void Menu();

	UPROPERTY(EditDefaultsOnly,Category = "Person|Input")
	TObjectPtr<UInputAction> IA_Menu;

/* 拾取 */
	UPROPERTY(EditDefaultsOnly, Category = "Person|Input")
	TObjectPtr<UInputAction> IA_Pickup;
	UPROPERTY(VisibleAnywhere, Category = "Person|Weapon")
	TObjectPtr<AWeapon> EquipWeapon;

	void Pickup();
	void PickupWeapon(AWeapon* Weapon);

/* 攻击 */
	UPROPERTY(EditDefaultsOnly, Category = "Person|Input")
	TObjectPtr<UInputAction> IA_Attack;

	UPROPERTY(EditDefaultsOnly, Category = "Person|Montage")
	TObjectPtr<UAnimMontage> AttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Person|Montage")
	TArray<FName> AttackMontageSections;

	void CharacterAttack();

/* 受击 */
	UPROPERTY(EditDefaultsOnly, Category = "Montage")
	TObjectPtr<UAnimMontage> GetHitMontage;

	void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter);
	void GetHitAnim(AActor* Hitter);
	double ObtainAngleForAnim(AActor* Hitter);
/* 状态 */
	UPROPERTY(EditDefaultsOnly, Category = "Person|Combat")
	TObjectPtr<UCombat> Combat;

	EWeaponState WeaponState = EWeaponState::EWS_Unequipped;
	EActionState ActionState = EActionState::EAS_Unoccupied;


/* 动画 */
	void PlayMontageSection(UAnimMontage* Montage, const FName& SectionName);
	void PlaySectionMontage(FName Section, UAnimMontage* Montage);
public:
	//获取
	AWeapon* GetEquippedWeapon() const;
};
