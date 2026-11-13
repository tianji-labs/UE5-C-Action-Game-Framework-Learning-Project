// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Character/StateType.h"
#include "Combat.generated.h"

class ATianji;
class UAttributeComponent;
class UAnimMontage;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class XIUXIAN_API UCombat : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCombat();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	UPROPERTY(VisibleAnywhere, Category = "Combat")
	TObjectPtr<AActor> Owner;
	UPROPERTY(VisibleAnywhere, Category = "Comabt")
	TObjectPtr<ATianji> Tianji;

/* 攻击 */
	UPROPERTY(EditDefaultsOnly, Category = "Montage")
	TObjectPtr<UAnimMontage> AttackMontage;
	UPROPERTY(EditDefaultsOnly, Category = "Montage")
	TArray<FName> AttackMontageSections;

	FOnMontageEnded AttackEndDelegate;

	void PlayAttackMontage();
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION()
	void SetWeaponCollision(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);
/* 死亡 */
	TWeakObjectPtr<UAttributeComponent> AttributeComponent;
	UPROPERTY(EditAnywhere, Category = "Montage")
	TObjectPtr<UAnimMontage> DeadMontage;

	void DeadAnim(AActor* Hitter);

/* 受击 */
	UPROPERTY(EditDefaultsOnly, Category = "Montage")
	TObjectPtr<UAnimMontage> GetHitMontage;

	FOnMontageEnded GetHitEndDelegate;

	void PlayGetHitMontage(AActor* Hitter);
	void OnGetHitMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	double ObtainAngleForAnim(AActor* Hitter);
/* 状态 */
	UPROPERTY(VisibleAnywhere, Category = "State")
	EPlayerCombatState PlayerCombatState = EPlayerCombatState::EPCS_Unoccupied;
	EPlayerLifeState PlayerLifeState = EPlayerLifeState::EPLS_Alive;
	EDeathPose DeathPose = EDeathPose::EDP_None;

/* 有限状态机 */
	void ChangeCombatState(EPlayerCombatState NewState);
	bool CanChangeState(EPlayerCombatState NewState);
	void ExitState();
	void EnterState(EPlayerCombatState NewState);


/* 通用 */
	void PlayMontageSection(UAnimMontage* Montage, const FName& SectionName);

public:
/* 攻击 */
	void HandleAttack();
/* 受击 */
	void HandleGetHit(AActor* Hitter);
/* 死亡 */
	void HandleDeath(AActor* Hitter);
/* 获取 */
	EPlayerCombatState GetPlayerCombatState() const { return PlayerCombatState; }
	EPlayerLifeState GetPlayerLifeState() const { return PlayerLifeState; } 
	UFUNCTION(BlueprintCallable)
	EDeathPose GetDeathPose() const { return DeathPose; }
};
