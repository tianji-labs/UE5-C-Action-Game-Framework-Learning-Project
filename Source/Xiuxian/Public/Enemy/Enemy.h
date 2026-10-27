// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/GetHitInterface.h"
#include"Character/StateType.h"
#include "Enemy.generated.h"

class UHealthBar;
class UHealthBarComponent;
class UAnimMontage;
class UEnemyPerception;
class UAISenseConfig_Sight;
class AAIController;
class UEnemyControllerComponent;
class UAttributeComponent;
class USphereComponent;
struct FTimerHandle;


UCLASS()
class XIUXIAN_API AEnemy : public ACharacter , public IGetHitInterface
{
	GENERATED_BODY()

public:

	AEnemy();
	virtual void Tick(float DeltaTime) override;

protected:

	virtual void BeginPlay() override;

private:
/* 通用变量 */
	UPROPERTY()
	TObjectPtr<AAIController> AIController;

/* AI */
	UPROPERTY(VisibleAnywhere, Category = "AI")
	TObjectPtr<UEnemyControllerComponent> EnemyControllerComponent;

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
	FTimerHandle AttackHandle;
	//攻击结束委托
	FOnMontageEnded AttackEndDelegate;
	UPROPERTY(EditAnywhere, Category = "Attack")
	double AttackRadius = 200.f;
	UPROPERTY(VisibleAnywhere, Category = "Observer")
	bool bIsAttackInProgress;

	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	void PlayAttackMontage();

/* 受击 */
	UPROPERTY(EditDefaultsOnly, Category = "GetHit")
	TObjectPtr<UAnimMontage> GetHitMontage;

	//FOnMontageEnded GetHitEndDelegate;

	//void OnGetHitMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter);
	void GetHitAnim(AActor* Hitter);
	//Damage
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

/* 死亡 */
	UPROPERTY(EditDefaultsOnly, Category = "Dead")
	TObjectPtr<UAnimMontage> DeadMontage;

	void DeadAnim(AActor* Hitter);

/* 血条 */
	UPROPERTY(VisibleAnywhere, Category = "Healthy")
	TObjectPtr<UHealthBar> HealthBar;

	UPROPERTY(VisibleAnywhere, Category = "Healthy")
	TObjectPtr<UHealthBarComponent> HealthBarComp;
/* 属性*/
	UPROPERTY(VisibleAnywhere, Category = "Attribute")
	TObjectPtr<UAttributeComponent> AttributeComponent;

/* 通用 */
	void PlaySectionMontage(FName Section, UAnimMontage* Montage);
	double ObtainAngleForAnim(AActor* Hitter);
/* 状态 */
	EDeathPose DeathPose = EDeathPose::EDP_DeathBack;


public:
/* 攻击 */ 
	void Attack(AActor* Target);
	void StopAttack();
	bool InTargetRange(AActor* Target, double Radius);

	FORCEINLINE bool IsAttackInProgress() const { return bIsAttackInProgress; }
	FORCEINLINE EDeathPose GetDeathPose() const { return DeathPose; }
};
