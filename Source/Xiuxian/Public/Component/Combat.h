// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Combat.generated.h"


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

	UFUNCTION()
	void AttackEnd(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);
};
