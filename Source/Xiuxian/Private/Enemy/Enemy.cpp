// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Enemy.h"
#include"Component/HealthBar.h"
#include"Component/HealthBarComponent.h"
//数学计算
//#include "Kismet/KismetMathLibrary.h"


AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
	HealthBarComp = CreateDefaultSubobject<UHealthBarComponent>(TEXT("HealthBarComp"));

	HealthBar = CreateDefaultSubobject<UHealthBar>(TEXT("HealBar"));
	HealthBar->SetupAttachment(GetRootComponent());


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
	GetHitAnim(Hitter);
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
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser); //自动广播
	return 0.0f;
}




