// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/GetHitInterface.h"
#include"Character/StateType.h"
#include "Enemy.generated.h"

class UAnimMontage;
class UEnemyPerception;
class UAISenseConfig_Sight;
class AAIController;
class UAttributeComponent;
class USphereComponent;
class AWeapon;
struct FTimerHandle;
class UBlackboardComponent;


UCLASS()
class XIUXIAN_API AEnemy : public ACharacter , public IGetHitInterface
{
	GENERATED_BODY()

public:

	AEnemy();
protected:

	virtual void BeginPlay() override;

private:
/* 通用变量 */
	UPROPERTY()
	TObjectPtr<AAIController> AIController;

/* 感知 */
	UPROPERTY(VisibleAnywhere, Category = "Perception")
	TObjectPtr<UEnemyPerception> EnemyPerception;
	UPROPERTY(VisibleAnywhere, Category = "Perception")
	TObjectPtr<UAISenseConfig_Sight> SightConfig;

/* 攻击 */
	UPROPERTY(VisibleAnywhere, Category = "Attack")
	TObjectPtr<USphereComponent> AttackRangeSphere; 
	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	TObjectPtr<UAnimMontage> AttackMontage;

	UPROPERTY(EditAnywhere, Category = "Attack")
	double AttackRadius = 170.f;
	UPROPERTY(VisibleAnywhere, Category = "Observer")
	bool bIsAttackInProgress;

	UFUNCTION()
	void AttackRangeBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	void AttackRangeEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	void PlayAttackMontage();

/* 受击 */
	UPROPERTY(EditDefaultsOnly, Category = "GetHit")
	TObjectPtr<UAnimMontage> GetHitMontage;


	void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter);
	void GetHitAnim(AActor* Hitter);

/* 死亡 */
	UPROPERTY(EditDefaultsOnly, Category = "Dead")
	TObjectPtr<UAnimMontage> DeadMontage;

	void DeadAnim(AActor* Hitter);

/* 属性*/
	UPROPERTY(VisibleAnywhere, Category = "Attribute")
	TObjectPtr<UAttributeComponent> AttributeComponent;

/* 武器 */
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<AWeapon> WeaponClass;
	TObjectPtr<AWeapon> DeafaultWeapon;

	void EquipDefaultWeapon();
	UFUNCTION()
	void SetWeaponCollidion(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);
/* 通用 */
	void PlaySectionMontage(FName Section, UAnimMontage* Montage);
	double ObtainAngleForAnim(AActor* Hitter);
/* 状态 */
	EDeathPose DeathPose = EDeathPose::EDP_DeathBack;

	UPROPERTY(VisibleAnywhere, Category = "Observer")
	EEnemyState EnemyState = EEnemyState::EES_NoState;

public:
/* 攻击 */ 
	void Die();
	bool InTargetRange(AActor* Target, double Radius);

/* Speed */
	void UpdateSpeed(float NewSpeed);
/* Blackboard */
	UBlackboardComponent* GetBlackboardComponent() const;

	FORCEINLINE bool IsAttackInProgress() const { return bIsAttackInProgress; }
	FORCEINLINE EDeathPose GetDeathPose() const { return DeathPose; }
	FORCEINLINE AWeapon* GetDeafaultWeapon() const { return DeafaultWeapon; }
	FORCEINLINE EEnemyState GetEnemyState() const { return EnemyState; }
	FORCEINLINE void SetEnemyState(EEnemyState NewState) { EnemyState = NewState; }
};
