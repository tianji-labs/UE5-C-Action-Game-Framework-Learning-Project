// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/Combat.h"
#include "AnimInstance/TianjiAnimInstance.h"
#include "Character/StateType.h"
#include "Character/Tianji.h"
#include"Item/Weapon.h"
#include "Component/AttributeComponent.h"


UCombat::UCombat()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UCombat::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UCombat::BeginPlay()
{
	Super::BeginPlay();

/*监听通知*/
	Owner = GetOwner();
	if (!IsValid(Owner)) return;
	Tianji = Cast<ATianji>(Owner);

	if (!IsValid(Tianji) || !IsValid(Tianji->GetMesh())) return;
	UAnimInstance* AnimInstance = Tianji->GetMesh()->GetAnimInstance();

	if (!IsValid(AnimInstance)) return;
	AnimInstance->OnPlayMontageNotifyBegin.AddUniqueDynamic(this, &UCombat::SetWeaponCollision);


}

void UCombat::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	//AttackEnd解绑
	ACharacter* CharOwner = Cast<ACharacter>(Owner);
	//不要用return跳过销毁
	if (IsValid(CharOwner) && IsValid(CharOwner->GetMesh()))
	{
		UAnimInstance* AnimInstance = CharOwner->GetMesh()->GetAnimInstance();
		if (IsValid(AnimInstance))
		{
			AnimInstance->OnPlayMontageNotifyBegin.RemoveAll(this);
		}
	}

	Super::EndPlay(EndPlayReason);
}

/* 攻击 */
void UCombat::HandleAttack()
{
	if (IsValid(Tianji) && Tianji->GetWeaponState() == EWeaponState::EWS_Unequipped) return;

	ChangeCombatState(EPlayerCombatState::EPCS_Attacking);
}

void UCombat::PlayAttackMontage()
{
	if (!IsValid(AttackMontage) || AttackMontageSections.Num() == 0) return;
	PlayMontageSection(AttackMontage, AttackMontageSections[0]);

	AttackEndDelegate.BindUObject(this, &UCombat::OnAttackMontageEnded);

	if(!IsValid(Tianji) || !IsValid(Tianji->GetMesh()) || !IsValid(Tianji->GetMesh()->GetAnimInstance())) return;
	Tianji->GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(AttackEndDelegate, AttackMontage);
}

void UCombat::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (bInterrupted == false)
	{
		PlayerCombatState = EPlayerCombatState::EPCS_Unoccupied;
	}
}

void UCombat::SetWeaponCollision(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
	ATianji* OwnerCharacter = Cast<ATianji>(Owner);
	if (!IsValid(OwnerCharacter) || !IsValid(OwnerCharacter->GetEquippedWeapon())) return;

	if (NotifyName == FName("EnableCollision"))
	{
		OwnerCharacter->GetEquippedWeapon()->SetWeaponCollisionEnable(true);
	}
	if (NotifyName == FName("UnableCollision"))
	{
		OwnerCharacter->GetEquippedWeapon()->SetWeaponCollisionEnable(false);
	}
}

/* 死亡 */ 
void UCombat::HandleDeath(AActor* Hitter)
{
	if (PlayerLifeState == EPlayerLifeState::EPLS_Dead) return;
	ChangeCombatState(EPlayerCombatState::EPCS_Unoccupied);
	PlayerLifeState = EPlayerLifeState::EPLS_Dead;
	DeadAnim(Hitter);
}

void UCombat::DeadAnim(AActor* Hitter)
{
	const double Theta = ObtainAngleForAnim(Hitter);

	FName Section = FName("Back");

	if (Theta <= -90.f || Theta >= 90.f)
	{
		Section = FName("Front");
		DeathPose = EDeathPose::EDP_DeathFront;
	}
	else
	{
		Section = FName("Back");
		DeathPose = EDeathPose::EDP_DeathBack;
	}

//	PlayMontageSection(DeadMontage,Section);
}

/* 受击 */
void UCombat::HandleGetHit(AActor* Hitter)
{
	PlayGetHitMontage(Hitter);

	ChangeCombatState(EPlayerCombatState::EPCS_HitReact);
}

void UCombat::PlayGetHitMontage(AActor* Hitter)
{

	const double Theta = ObtainAngleForAnim(Hitter);

	FName Section = FName("FromBack");

	if (Theta < 45.f && Theta > -45.f)
	{
		Section = FName("FromFront");
	}
	else if (Theta < -135.f && Theta > 135.f)
	{
		Section = FName("FromBack");
	}
	else if (Theta <= -45.f && Theta >= -135.f)
	{
		Section = FName("FromLeft");
	}
	else if (Theta <= 135.f && Theta >= 45.f)
	{
		Section = FName("FromRight");
	}

	if (!IsValid(GetHitMontage)) return;
	PlayMontageSection(GetHitMontage, Section);

	GetHitEndDelegate.BindUObject(this, &UCombat::OnGetHitMontageEnded);

	if (!IsValid(Tianji) || !IsValid(Tianji->GetMesh()) || !IsValid(Tianji->GetMesh()->GetAnimInstance())) return;
	Tianji->GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(GetHitEndDelegate, GetHitMontage);
}

double UCombat::ObtainAngleForAnim(AActor* Hitter)
{
	//获取自身单位向量
	if (!IsValid(Tianji))
	{
		UE_LOG(LogTemp, Warning, TEXT("UCombat::ObtainAngleForAnim: Tianji is not valid."));
		return 0.0;
	}
	const FVector Forward = Tianji->GetActorForwardVector();
	//使向量处于同一平面
	const FVector HitterLocation = Hitter->GetActorLocation();
	const FVector PointLowerd = FVector(HitterLocation.X, HitterLocation.Y, Tianji->GetActorLocation().Z);
	//获得自身到Hitter的单位向量
	const FVector ToHit = (PointLowerd - Tianji->GetActorLocation()).GetSafeNormal();

	//单位向量点积获得cos
	double CosTheta = FVector::DotProduct(Forward, ToHit);
	//cos转弧度
	CosTheta = FMath::Clamp(CosTheta, -1.0, 1.0);
	double Theta = FMath::Acos(CosTheta);
	//弧度转角度
	Theta = FMath::RadiansToDegrees(Theta);

	//右手坐标系调整方向
	const FVector CrossProduct = FVector::CrossProduct(HitterLocation, ToHit);
	if (CrossProduct.Z < 0)
	{
		Theta *= -1.f;
	}

	return Theta;
}

void UCombat::OnGetHitMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (bInterrupted == true)
	{
		UE_LOG(LogTemp, Warning, TEXT("UCombat::OnGetHitMontageEnded: Montage was interrupted."));
		return;
	}
	ChangeCombatState(EPlayerCombatState::EPCS_Unoccupied);
}

/* 有限状态机 */
void UCombat::ChangeCombatState(EPlayerCombatState NewState)
{
	if (PlayerCombatState == NewState) return;

	if (!CanChangeState(NewState)) return;
	ExitState();
	PlayerCombatState = NewState;
	EnterState(NewState);

}

bool UCombat::CanChangeState(EPlayerCombatState NewState)
{
	if (PlayerLifeState == EPlayerLifeState::EPLS_Dead)
	{
		return false;
	}

	if (NewState == EPlayerCombatState::EPCS_HitReact)
	{
		return true;
	}
	

	switch (PlayerCombatState)
	{
		case EPlayerCombatState::EPCS_Unoccupied:
			return true;

		case EPlayerCombatState::EPCS_HitReact:
			if (NewState == EPlayerCombatState::EPCS_Unoccupied)
			{
				return true;
			}
			else
			{
				return false;
			}

		case EPlayerCombatState::EPCS_Attacking:
			return true;

		case EPlayerCombatState::EPCS_Dodging:
			return false;
		default:
			return false;
	}
}

void UCombat::ExitState()
{
	EPlayerCombatState OldState = PlayerCombatState;
	switch (OldState)
	{
	case(EPlayerCombatState::EPCS_Unoccupied):
	{
		return;
	}
	case(EPlayerCombatState::EPCS_Attacking):
	{
		return;
	}
	case(EPlayerCombatState::EPCS_HitReact):
	{
		return;
	}
	case(EPlayerCombatState::EPCS_Dodging):
	{
		return;
	}
	default:
		return;
	}
}

void UCombat::EnterState(EPlayerCombatState NewState)
{
	switch (NewState)
	{
	case(EPlayerCombatState::EPCS_Unoccupied):
	{
		return;
	}
	case(EPlayerCombatState::EPCS_Attacking):
	{
		PlayAttackMontage();
		return;
	}
	case(EPlayerCombatState::EPCS_HitReact):
	{
		return;
	}
	case(EPlayerCombatState::EPCS_Dodging):
	{
		return;
	}
	default:
		return;
	}
}
//通用
void UCombat::PlayMontageSection(UAnimMontage* Montage, const FName& SectionName)
{
	if (!IsValid(Tianji) && !IsValid(Tianji->GetMesh())) return;
	UAnimInstance* AnimInstance = Tianji->GetMesh()->GetAnimInstance();

	if (!IsValid(AnimInstance) || !IsValid(Montage)) return;
	AnimInstance->Montage_Play(Montage);
	AnimInstance->Montage_JumpToSection(SectionName, Montage);

}