
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
}

float UAttributeComponent::GetHealthBarPercent() const
{
		return CurrentHealth / MaxHealth;
}

void UAttributeComponent::TakeDamage(float DamageAmount)
{
	CurrentHealth = FMath::Clamp(CurrentHealth- DamageAmount, 0.f, MaxHealth);
}






