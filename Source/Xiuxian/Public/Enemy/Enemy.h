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
class UEnemyAttributeComponent;
class USphereComponent;
class AWeapon;
struct FTimerHandle;
class UBlackboardComponent;
class UHealthBarWidgetComponent;


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

	UFUNCTION()
	void AttackRangeBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void AttackRangeEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


/* 受击 */
	UPROPERTY(EditDefaultsOnly, Category = "GetHit")
	TObjectPtr<UAnimMontage> GetHitMontage;


	void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter);


/* 死亡 */
	UPROPERTY(EditDefaultsOnly, Category = "Dead")
	TObjectPtr<UAnimMontage> DeadMontage;

/* 巡逻 */
	UPROPERTY(EditAnywhere, Category = "Patrol")
	TArray<TObjectPtr<AActor>> PatrolTargets;

/* 属性*/
	UPROPERTY(VisibleAnywhere, Category = "Attribute")
	TObjectPtr<UEnemyAttributeComponent> EnemyAttributeComponent;

/* UI */
	UPROPERTY(VisibleAnywhere, Category = "HealthBarWidgetComponent")
	TObjectPtr<UHealthBarWidgetComponent> HealthBarWidgetComponent;
/* 武器 */
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<AWeapon> WeaponClass;
	TObjectPtr<AWeapon> DeafaultWeapon;

	void EquipDefaultWeapon();
	UFUNCTION()
	void SetWeaponCollidion(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);
/* 通用 */
	bool PlaySectionMontage(FName Section, UAnimMontage* Montage);
	double ObtainAngleForAnim(AActor* Hitter);
/* 状态 */
	EDeathPose DeathPose = EDeathPose::EDP_DeathBack;

	UPROPERTY(VisibleAnywhere, Category = "Observer")
	EEnemyState EnemyState = EEnemyState::EES_NoState;

public:
//攻击
	bool PlayAttackMontage(const FOnMontageEnded& EndDelegate);
//受击
	bool PlayGetHitMontage(AActor* Hitter,const FOnMontageEnded& EndDelegate);
//死亡
	void PlayDeathMontage(AActor* Hitter);
	void Die();
//巡逻
	FORCEINLINE TArray<TObjectPtr<AActor>> GetPatrolTargets() const { return PatrolTargets; }
//速度
	void UpdateSpeed(float NewSpeed);
//黑板
	UBlackboardComponent* GetBlackboardComponent() const;

	FORCEINLINE EDeathPose GetDeathPose() const { return DeathPose; }
	FORCEINLINE AWeapon* GetDeafaultWeapon() const { return DeafaultWeapon; }
	FORCEINLINE EEnemyState GetEnemyState() const { return EnemyState; }
	FORCEINLINE void SetEnemyState(EEnemyState NewState) { EnemyState = NewState; }
};
