#pragma once

UENUM(BlueprintType)
enum class EWeaponState : uint8
{
	EWS_Unequipped UMETA(DisplayName = "Unequip"),
	EWS_CloseCombatOneHand UMETA(DisplayName = "CloseCombatOneHand")
};