// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/GetHitInterface.h"
#include "Enemy.generated.h"

class UHealthBar;
class UHealthBarComponent;
class UAnimMontage;

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
/* ÊÜ»÷ */
	UPROPERTY(EditDefaultsOnly, Category = "Person|GetHit")
	TObjectPtr<UAnimMontage> GetHitMontage;
	UPROPERTY(VisibleAnywhere, Category = "Person")
	FName Sction;

	void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter);
	void GetHitAnim(AActor* Hitter);
	//Damage
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

/* ÑªÌõ */
	UPROPERTY(VisibleAnywhere, Category = "Person|Healthy")
	TObjectPtr<UHealthBar> HealthBar;

	UPROPERTY(VisibleAnywhere, Category = "Person|Healthy")
	TObjectPtr<UHealthBarComponent> HealthBarComp;
};
