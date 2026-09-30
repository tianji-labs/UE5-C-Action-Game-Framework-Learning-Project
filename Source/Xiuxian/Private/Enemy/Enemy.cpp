// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Enemy.h"
#include"Components/CapsuleComponent.h"
//移动组件
#include "GameFramework/CharacterMovementComponent.h"
//组件
#include "Component/EnemyControllerComponent.h"
#include "Component/EnemyPerception.h"
#include"Perception/AISenseConfig_Sight.h"
#include"Component/HealthBar.h"
#include"Component/HealthBarComponent.h"

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
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();

}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemy::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	if (HealthBarComp->IsAlive())
	{
		GetHitAnim(Hitter);

	}
	else
	{
		DeadAnim(Hitter);
		EnemyState = EEnemyState::EES_Dead;
		SetLifeSpan(5.f);
		GetCharacterMovement()->bOrientRotationToMovement = false;
		if (IsValid(EnemyPerception))
		{
			EnemyPerception->ClearPerception();
		}

		if (IsValid(GetCapsuleComponent()))
		{
			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}

	}
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
	if (EnemyState == EEnemyState::EES_Dead) return 0.f;
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser); //自动广播

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

void AEnemy::PlaySectionMontage(FName Section, UAnimMontage* Montage)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (!IsValid(AnimInstance) || !IsValid(Montage)) return;
	AnimInstance->Montage_Play(Montage);
	AnimInstance->Montage_JumpToSection(Section, Montage);
}






