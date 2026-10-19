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

UENUM(BlueprintType)
enum class EDeathPose : uint8
{
	EDP_DeathBack UMETA(DisplayName = "DeathBack"),
	EDP_DeathFront UMETA(DisplayName = "DeathFront")
};

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	EES_NoState UMETA(DisplayName = "NoState"),
	EES_Dead UMETA(DisplayName = "Dead"),
	EES_Attacking UMETA(DisplayName = "Attacking"),
	EES_Patrolling UMETA(DisplayName = "Patrolling"),
	EES_Chasing UMETA(DisplayName = "Chasing")
};