#pragma once

UENUM(BlueprintType)
enum class EWeaponState : uint8
{
	EWS_Unequipped UMETA(DisplayName = "Unequip"),
	EWS_CloseCombatOneHand UMETA(DisplayName = "CloseCombatOneHand")
};

UENUM(BlueprintType)
enum class EActionState : uint8
{
	EAS_Unoccupied UMETA(DisplayName = "Unoccupied"),
	EAS_Attacking UMETA(DisplayName = "Attacking")
};