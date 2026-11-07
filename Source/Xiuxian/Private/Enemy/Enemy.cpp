// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Enemy.h"
//移动组件
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SphereComponent.h"
//组件
#include"Components/CapsuleComponent.h"
#include "Component/EnemyControllerComponent.h"
#include "Component/EnemyPerception.h"
#include"Perception/AISenseConfig_Sight.h"
#include"Component/HealthBar.h"
#include"Component/HealthBarComponent.h"
#include"Component/AttributeComponent.h"
//武器
#include"Item/Weapon.h"
//动画
#include"AnimInstance/EnemyAnimInstance.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
	HealthBarComp = CreateDefaultSubobject<UHealthBarComponent>(TEXT("HealthBarComp"));
	//血条
	HealthBar = CreateDefaultSubobject<UHealthBar>(TEXT("HealBar"));
	HealthBar->SetupAttachment(GetRootComponent());
	//感知
	EnemyPerception = CreateDefaultSubobject<UEnemyPerception>(TEXT("EnemyPerception"));
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	SightConfig->SightRadius = 1000.f; //视距半径（能看多远）
	SightConfig->LoseSightRadius = 1200.f;
	SightConfig->PeripheralVisionAngleDegrees = 45.f;//能看到多大角度
	SightConfig->DetectionByAffiliation.bDetectEnemies = false;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true; //检测中立
	SightConfig->DetectionByAffiliation.bDetectFriendlies = false;//检测友方
	EnemyPerception->ConfigureSense(*SightConfig);//配置感知（sense）
	EnemyPerception->SetDominantSense(SightConfig->GetSenseImplementation());
	/*
	GetSenseImplementation 获得配置对应的 Sense 类型  AISense_Sight
	SetDominantSense 把 Sense 设置为主要/主导感知类型
	*/
	EnemyControllerComponent = CreateDefaultSubobject<UEnemyControllerComponent>(TEXT("EnemyControllerComponent"));
	//属性
	AttributeComponent = CreateDefaultSubobject<UAttributeComponent>(TEXT("AttributeComp"));
	//攻击范围
	AttackRangeSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AttackRangeSphere"));
	AttackRangeSphere->SetupAttachment(GetRootComponent());
	AttackRangeSphere->InitSphereRadius(AttackRadius);
	
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	Tags.Add(FName("Enemy"));

	EquipDefaultWeapon();

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (!IsValid(AnimInstance)) return;
	AnimInstance->OnPlayMontageNotifyBegin.AddUniqueDynamic(this, &AEnemy::SetWeaponCollidion);

}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

/* 攻击 */
void AEnemy::Attack(AActor* Target)
{
	GetWorldTimerManager().SetTimer(AttackHandle, this, &AEnemy::PlayAttackMontage, 1.f, false);
}

void AEnemy::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	bIsAttackInProgress = false;

	if (!IsValid(EnemyControllerComponent)) return;
	EnemyControllerComponent->UpdateBehaviorState();

	if (!IsValid(EnemyControllerComponent->GetAttackTarget()) || AttributeComponent->IsDead()) return;
	Attack(EnemyControllerComponent->GetAttackTarget());
}

void AEnemy::StopAttack()
{
	GetWorldTimerManager().ClearTimer(AttackHandle);
}


void AEnemy::PlayAttackMontage()
{
	//播放攻击蒙太奇
	if (!IsValid(AttackMontage)) return;
	PlayAnimMontage(AttackMontage);

	bIsAttackInProgress = true;

	//设置蒙太奇结束委托
	AttackEndDelegate.BindUObject(this, &AEnemy::OnAttackMontageEnded);

	if(!IsValid(GetMesh()) || !IsValid(GetMesh()->GetAnimInstance())) return;
	GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(AttackEndDelegate, AttackMontage);
}

bool AEnemy::InTargetRange(AActor* Target, double Radius)
{
	float Distance = FVector::Distance(GetActorLocation(), Target->GetActorLocation());
	return Distance <= Radius;
}

/* 受击 */
void AEnemy::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	if (!AttributeComponent->IsDead())
	{
		GetHitAnim(Hitter);

		if (!IsValid(EnemyControllerComponent)) return;
		if(InTargetRange(Hitter, AttackRadius))
		{
			EnemyControllerComponent->SetChaseTarget(Hitter);
			EnemyControllerComponent->SetAttackTarget(Hitter);
			EnemyControllerComponent->UpdateBehaviorState();
		}
		else
		{
			EnemyControllerComponent->SetChaseTarget(Hitter);
			EnemyControllerComponent->UpdateBehaviorState();
		}
	}
	else
	{
		DeadAnim(Hitter);
		if (!IsValid(EnemyControllerComponent)) return;
		EnemyControllerComponent->UpdateBehaviorState();
	}
}

void AEnemy::Die()
{

	if (IsValid(AttributeComponent))
	{
		EnemyControllerComponent->UpdateBehaviorState();
	}

	SetLifeSpan(5.f);

	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	bUseControllerRotationYaw = false;

	if (IsValid(EnemyPerception))
	{
		EnemyPerception->ClearPerception();
	}

	if (IsValid(GetCapsuleComponent()))
	{
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	DeafaultWeapon->SetBoxNoCollision();
	DeafaultWeapon->SetLifeSpan(5.f);
}

void AEnemy::GetHitAnim(AActor* Hitter)
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

	PlaySectionMontage(Section, GetHitMontage);
}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (!IsValid(AttributeComponent)|| EnemyState==EEnemyState::EES_Dead) return 0.f;
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser); //自动广播给TakeAnyDamage
	return 0.0f;
}

void AEnemy::DeadAnim(AActor* Hitter)
{
	const double Theta = ObtainAngleForAnim(Hitter);

	FName Section = FName("ForBack");

	if (Theta <= -90.f || Theta >= 90.f)
	{
		Section = FName("ForFront");
		DeathPose = EDeathPose::EDP_DeathFront;
	}
	else
	{
		Section = FName("ForBack");
		DeathPose = EDeathPose::EDP_DeathBack;
	}

	PlaySectionMontage(Section, DeadMontage);
}

double AEnemy::ObtainAngleForAnim(AActor* Hitter)
{
	//获取自身单位向量
	const FVector Forward = GetActorForwardVector();
	//使向量处于同一平面
	const FVector HitterLocation = Hitter->GetActorLocation();
	const FVector PointLowerd = FVector(HitterLocation.X, HitterLocation.Y, GetActorLocation().Z);
	//获得自身到Hitter的单位向量
	const FVector ToHit = (PointLowerd - GetActorLocation()).GetSafeNormal();

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

/* 武器 */
void AEnemy::EquipDefaultWeapon()
{
	UWorld* World = GetWorld();
	if (World && IsValid(WeaponClass))
	{
		DeafaultWeapon = World->SpawnActor<AWeapon>(WeaponClass);
		DeafaultWeapon->Equip(GetMesh(), FName("RightHandWeapon"), this, this);
		DeafaultWeapon->SetSphereNocollision();
	}
}

void AEnemy::SetWeaponCollidion(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
	if (!IsValid(DeafaultWeapon)) return;

	if (NotifyName == FName("EnableCollision"))
	{

		DeafaultWeapon->SetWeaponCollisionEnable(true);
	}
	if (NotifyName == FName("UnableCollision"))
	{
		DeafaultWeapon->SetWeaponCollisionEnable(false);
	}
}
/* 通用 */
void AEnemy::PlaySectionMontage(FName Section, UAnimMontage* Montage)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (!IsValid(AnimInstance) || !IsValid(Montage)) return;
	AnimInstance->Montage_Play(Montage);
	AnimInstance->Montage_JumpToSection(Section, Montage);
}






