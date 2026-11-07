
#include "Component/AttributeComponent.h"


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






