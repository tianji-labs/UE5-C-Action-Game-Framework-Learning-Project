
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Character/StateType.h"
#include "EnemyControllerComponent.generated.h"

class AEnemyAIController;
class UEnemyPerception;
class AEnemy;
class UAttributeComponent;
class USphereComponent;



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class XIUXIAN_API UEnemyControllerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UEnemyControllerComponent();

protected:
	virtual void BeginPlay() override;


private:
	UPROPERTY()
	TObjectPtr<AActor> Owner;
	UPROPERTY()
	TObjectPtr<AEnemy> Enemy;
/* 巡逻 */
	UPROPERTY(EditAnywhere, Category = "Patrolling")
	TArray<TObjectPtr<AActor>> PatrolPoints;
	UPROPERTY()
	TObjectPtr<AActor> PatrolTarget;


	UPROPERTY()
	TObjectPtr<AEnemyAIController> EnemyAIController;
	UPROPERTY()
	TObjectPtr<UEnemyPerception> EnemyPerception;
	UPROPERTY()
	TObjectPtr<UAttributeComponent> AttributeComponent;
	UPROPERTY()
	TObjectPtr<AActor> ChaseTarget;
	UPROPERTY()
	TWeakObjectPtr<USphereComponent> AttackRangeSphere;
	UPROPERTY()
	TObjectPtr<AActor> AttackTarget;

	void ReceivePerceptionUpdated(AActor* FoundActor,bool bSensed);
	UFUNCTION()
	void OnAttackRangeBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnAttackRangeEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

/* 有限状态机 */
	bool CanChangeState(EEnemyState NewState);
	void ExitState(EEnemyState OldState);
	void EnterState(EEnemyState NewState);
public:	
	FORCEINLINE TArray<TObjectPtr<AActor>> GetPatrolPoints() const { return PatrolPoints; }
	FORCEINLINE void SetAttackTarget(AActor* NewAttackTarget) { AttackTarget = NewAttackTarget; }
	FORCEINLINE void SetChaseTarget(AActor* NewChaseTarget) { ChaseTarget = NewChaseTarget; }
	FORCEINLINE TObjectPtr<AActor> GetChaseTarget() const { return ChaseTarget; }
	FORCEINLINE TObjectPtr<AActor> GetAttackTarget() const { return AttackTarget; }

	void InitializeAI(AEnemyAIController* InEnemyAIController);

/* 有限状态机 */
	void UpdateBehaviorState();
	void ChangeEnemyState(EEnemyState NewState);




};
