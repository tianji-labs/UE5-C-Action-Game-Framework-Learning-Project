
#include "Component/AttributeComponent.h"

#include "Kismet/KismetMathLibrary.h"


UAttributeComponent::UAttributeComponent()
{

	PrimaryComponentTick.bCanEverTick = false;

}

void UAttributeComponent::BeginPlay()
{
	Super::BeginPlay();
	
	CurrentHealth = MaxHealth;

	if (!IsValid(GetOwner())) return;
	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UAttributeComponent::HandleTakeAnyDamage);
}

void UAttributeComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	if (!IsValid(GetOwner())) return;
	GetOwner()->OnTakeAnyDamage.RemoveDynamic(this, &UAttributeComponent::HandleTakeAnyDamage);
}

void UAttributeComponent::HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (IsDead()) return;
	CurrentHealth = FMath::Clamp(CurrentHealth- Damage, 0.f, MaxHealth);

	float HealthPercent = GetHealthBarPercent();

	OnHealthChanged.Broadcast(HealthPercent);
}


float UAttributeComponent::GetHealthBarPercent() const
{
		return CurrentHealth / MaxHealth;
}




