// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Enemy.h"
#include"Component/HealthBar.h"
#include"Component/HealthBarComponent.h"
//perception
#include "Component/EnemyPerception.h"
#include"Perception/AISenseConfig_Sight.h"
//数学计算
//#include "Kismet/KismetMathLibrary.h"


AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
	HealthBarComp = CreateDefaultSubobject<UHealthBarComponent>(TEXT("HealthBarComp"));

	HealthBar = CreateDefaultSubobject<UHealthBar>(TEXT("HealBar"));
	HealthBar->SetupAttachment(GetRootComponent());


	EnemyPerception = CreateDefaultSubobject<UEnemyPerception>(TEXT("EnemyPerception"));
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	SightConfig->SightRadius = 1500.f; //视距半径（能看多远）
	SightConfig->LoseSightRadius = 1800.f;
	SightConfig->PeripheralVisionAngleDegrees = 60.f;//能看到多大角度
	//检测从属（阵营）
	SightConfig->DetectionByAffiliation.bDetectEnemies = false;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true; //检测中立
	SightConfig->DetectionByAffiliation.bDetectFriendlies = false;//检测友方
	EnemyPerception->ConfigureSense(*SightConfig);//配置感知（sense）
	EnemyPerception->SetDominantSense(SightConfig->GetSenseImplementation());
	/*
	GetSenseImplementation 获得配置对应的 Sense 类型  AISense_Sight
	SetDominantSense 把 Sense 设置为主要/主导感知类型
	*/
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	EnemyPerception->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemy::OnTargetPerceptionUpdated);
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemy::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus) //Stimulus 一份感知结果
{
	if (Stimulus.WasSuccessfullySensed())
	{
		UE_LOG(LogTemp, Warning, TEXT("Found:%s"), *Actor->GetName());
	}
	else if (!Stimulus.WasSuccessfullySensed())
	{
		UE_LOG(LogTemp, Warning, TEXT("NoFound"));
	}

}

void AEnemy::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	if (EnemyState != EEnemyState::EES_Dead)
	{
		GetHitAnim(Hitter);
	}
}

void AEnemy::GetHitAnim(AActor* Hitter)
{
	//获取自身单位向量
	const FVector Forward = GetActorForwardVector();
	//使向量处于同一平面
	FVector HitterLocation = Hitter->GetActorLocation();
	const FVector PointLowerd = FVector(HitterLocation.X, HitterLocation.Y, GetActorLocation().Z);
	//获得自身到Hitter的单位向量
	const FVector ToHit = (PointLowerd - GetActorLocation()).GetSafeNormal();

	//单位向量点积获得cos
	double CosTheta = FVector::DotProduct(Forward,ToHit);
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

	Sction = FName("FromBack");
	if (Theta < 45.f && Theta > -45.f)
	{
		Sction = FName("FromFront");
	}
	else if (Theta < -135.f && Theta > 135.f)
	{
		Sction = FName("FromBack");
	}
	else if (Theta <= -45.f && Theta >= -135.f)
	{
		Sction = FName("FromLeft");
	}
	else if (Theta <= 135.f && Theta >= 45.f)
	{
		Sction = FName("FromRight");
	}

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (!IsValid(AnimInstance) || !IsValid(GetHitMontage)) return;
	AnimInstance->Montage_Play(GetHitMontage);
	AnimInstance->Montage_JumpToSection(Sction, GetHitMontage);
}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (EnemyState == EEnemyState::EES_Dead) return 0.f;
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser); //自动广播

	if (IsValid(HealthBarComp))
	{
		if (!(HealthBarComp->IsAlive()))
		{
			EnemyState = EEnemyState::EES_Dead;
		}
	}	
	
	return 0.0f;
}




