// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/GetHitInterface.h"
#include "Enemy.generated.h"

class UHealthBar;
class UHealthBarComponent;
class UAnimMontage;
class UEnemyPerception;
class UAISenseConfig_Sight;
class AAIController;

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	EES_NoState UMETA(DisplayName = "NoState"),
	EES_Dead UMETA(DisplayName = "Dead"),
	EES_Chasing UMETA(DisplayName = "Chasing")
};

UENUM(BlueprintType)
enum class EDeathPose : uint8
{
	EDP_DeathBack UMETA(DisplayName = "DeathBack"),
	EDP_DeathFront UMETA(DisplayName = "DeathFront")
};

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

/* 感知 */

	UPROPERTY(VisibleAnywhere, Category = "Perception")
	TObjectPtr<UEnemyPerception> EnemyPerception;
	UPROPERTY(VisibleAnywhere, Category = "Perception")
	TObjectPtr<UAISenseConfig_Sight> SightConfig;

/* 追击 */
	FTimerHandle ChaseTimer;

	void MoveToTarget(AActor* Target);


/* 受击 */
	UPROPERTY(EditDefaultsOnly, Category = "GetHit")
	TObjectPtr<UAnimMontage> GetHitMontage;

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

/* 通用 */
	void PlaySectionMontage(FName Section, UAnimMontage* Montage);
	double ObtainAngleForAnim(AActor* Hitter);
public:
/* 状态 */
	UPROPERTY(BlueprintReadOnly)
	EEnemyState EnemyState = EEnemyState::EES_NoState;
	UPROPERTY(BlueprintReadOnly)
	EDeathPose DeathPose = EDeathPose::EDP_DeathBack;

/* Chasing */
	void StartChaseTarget(AActor* Target);
	void EndChaseTarget();
};
