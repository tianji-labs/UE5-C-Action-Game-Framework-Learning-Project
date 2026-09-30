
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyControllerComponent.generated.h"

class AEnemyAIController;
class UEnemyPerception;
class AEnemy;


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
	TObjectPtr<AEnemy> Enemy;
	UPROPERTY(EditAnywhere, Category = "Patrolling")
	TArray<TObjectPtr<AActor>> PatrolPoints;
	UPROPERTY()
	TObjectPtr<AEnemyAIController> EnemyAIController;
	UPROPERTY()
	TObjectPtr<UEnemyPerception> EnemyPerception;

	void ReceivePerceptionUpdated(AActor* FoundActor,bool bSensed);

public:	
	FORCEINLINE TArray<TObjectPtr<AActor>> GetPatrolPoints() const { return PatrolPoints; }

	void InitializeAI(AEnemyAIController* InEnemyAIController);

	void UpdateBehaviorState();

};
