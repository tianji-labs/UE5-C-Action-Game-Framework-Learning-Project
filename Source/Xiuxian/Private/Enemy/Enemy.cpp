// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Enemy.h"
#include"Component/HealthBar.h"
#include"Component/HealthBarComponent.h"


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
	UE_LOG(LogTemp, Warning, TEXT("Enemy GetHit"));
}


float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser); //×Ô¶¯¹ã²¥
	return 0.0f;
}




