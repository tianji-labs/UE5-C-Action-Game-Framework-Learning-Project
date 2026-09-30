
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributeComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class XIUXIAN_API UAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAttributeComponent();

protected:
	virtual void BeginPlay() override;

private:
	//血条
	UPROPERTY(EditAnywhere, Category = "Attribute")
	float MaxHealth = 100.f;
	UPROPERTY(VisibleAnywhere, Category = "Attribute")
	float CurrentHealth = 100.f;
	//速度
	UPROPERTY(EditAnywhere, Category = "Attribute")
	float MaxSpeed = 600.f;

public:	
/* 获取 */
	//血条
	FORCEINLINE float GetMaxHealth() const { return MaxHealth; }
	FORCEINLINE float GetCurrentHealth() const { return CurrentHealth; }
	//AI
	FORCEINLINE float GetMaxSpeed() const { return MaxSpeed; }

/* 设置 */
	//血条
	FORCEINLINE void SetMaxHealth(float NewMaxHealth) { MaxHealth = NewMaxHealth; }
	//AI
	FORCEINLINE void SetMaxSpeed(float NewMaxSpeed) { MaxSpeed = NewMaxSpeed; }

};
