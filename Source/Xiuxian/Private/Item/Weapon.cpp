// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Weapon.h"
#include"Components/SphereComponent.h"
#include"Character/Tianji.h"


AWeapon::AWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	SetRootComponent(MeshComponent);

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetupAttachment(MeshComponent);
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	//重叠
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnSphereBeginOverlap);
	SphereComponent->OnComponentEndOverlap.AddDynamic(this, &AWeapon::OnSphereEndOverlap);
	
}

void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AWeapon::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ATianji* Tianji = CastChecked<ATianji>(OtherActor);
	if (Tianji)
	{
		Tianji->SetOverlappingItem(this);
	}
}

void AWeapon::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ATianji* Tianji = CastChecked<ATianji>(OtherActor);
	if (Tianji)
	{
		Tianji->SetOverlappingItem(nullptr);
	}
}

//拾取武器
void AWeapon::AttachWeapon(USceneComponent* InParent, FName InSocketName, AActor* NewOwner, APawn* NewInstigator)
{
	//拾取
	AttachMeshToSocket(InParent, InSocketName);
	//拥有者与发起者
	SetOwner(NewOwner);
	SetInstigator(NewInstigator);
}

void AWeapon::AttachMeshToSocket(USceneComponent* InParent, const FName& InSocketName)
{
	FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
	MeshComponent->AttachToComponent(InParent, TransformRules, InSocketName);
}