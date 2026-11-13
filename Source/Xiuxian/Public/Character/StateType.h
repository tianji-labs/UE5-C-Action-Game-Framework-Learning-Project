#pragma once

UENUM(BlueprintType)
enum class EWeaponState : uint8
{
	EWS_Unequipped UMETA(DisplayName = "Unequip"),
	EWS_CloseCombatOneHand UMETA(DisplayName = "CloseCombatOneHand")
};

UENUM(BlueprintType)
enum class EPlayerLifeState : uint8
{
	EPLS_Alive UMETA(DisplayName = "Alive"),
	EPLS_Dead UMETA(DisplayName = "Dead")
};

UENUM(BlueprintType)
enum class EPlayerCombatState : uint8
{
	EPCS_Unoccupied UMETA(DisplayName = "Unoccupied"),
	EPCS_HitReact UMETA(DisplayName = "HitReact"),
	EPCS_Attacking UMETA(DisplayName = "Attacking"),
	EPCS_Dodging UMETA(DisplayName = "Dodging")

};


UENUM(BlueprintType)
enum class EDeathPose : uint8
{
	EDP_None UMETA(DisplayName = "None"),
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