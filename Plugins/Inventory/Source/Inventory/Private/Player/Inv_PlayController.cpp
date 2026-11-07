


#include "Player/Inv_PlayController.h"
//增强输入
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
//HUD
#include "HUD/Widget/Inv_HUDWidget.h"

//信息
#include"Kismet/GameplayStatics.h"
#include <Item/Component/Inv_ItemComponent.h>
//高亮
#include "Interaction/Inv_Highlightable.h"



AInv_PlayController::AInv_PlayController()
{
	PrimaryActorTick.bCanEverTick = true;

	TraceLength = 1000.0;

	ItemTraceChannel = ECC_GameTraceChannel1;
}

void AInv_PlayController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TraceForItem();
}

void AInv_PlayController::BeginPlay()
{
	Super::BeginPlay();

	//需要模块"EnhancedInput"
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (IsValid(Subsystem))
	{
		Subsystem->AddMappingContext(IMC_Inventory, 0);
		Subsystem->AddMappingContext(IMC_Tianji, 1);
	}

	CreateHUDWidget();

}

void AInv_PlayController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

	EnhancedInputComponent->BindAction(PrimaryInteractAction, ETriggerEvent::Started, this, &AInv_PlayController::PrimaryInteract);
}

void AInv_PlayController::TraceForItem()
{
	//获得屏幕中心位置
	if (!IsValid(GEngine) || !IsValid(GEngine->GameViewport)) return;
	FVector2D ViewportSize;
	GEngine->GameViewport->GetViewportSize(ViewportSize);
	const FVector2D ViewportCenter = ViewportSize / 2.f;

	//计算相机位置及单位向量
	FVector TraceStart;
	FVector Forward;
	if (!UGameplayStatics::DeprojectScreenToWorld(this,ViewportCenter,TraceStart,Forward)) return;
	const FVector TraceEnd = TraceStart + Forward * TraceLength;
	//碰撞结果
	FHitResult HitResult;
	GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ItemTraceChannel);

	//显示或隐藏物品信息
	LastActor = ThisActor;
	ThisActor = HitResult.GetActor();

	if (!IsValid(ThisActor.Get()))
	{
		if (IsValid(HUDWidget)) HUDWidget->HidePickupMessage();
	}

	if (LastActor == ThisActor) return;

	if (IsValid(ThisActor.Get()))
	{
		//获取角色的高亮接口
		UActorComponent* Highlightable = ThisActor->FindComponentByInterface(UInv_Highlightable::StaticClass());
		if (IsValid(Highlightable))
		{
			IInv_Highlightable::Execute_Highlight(Highlightable);
		}
		//信息
		UInv_ItemComponent* ItemComponent = ThisActor->FindComponentByClass<UInv_ItemComponent>();
		if (!IsValid(ItemComponent))return;

		if (IsValid(HUDWidget)) HUDWidget->ShowPickupMessage(ItemComponent->GetPickupMessage());

	}
	if (IsValid(LastActor.Get()))
	{
		UActorComponent* Highlightable = LastActor->FindComponentByInterface(UInv_Highlightable::StaticClass());
		if (IsValid(Highlightable))
		{
			IInv_Highlightable::Execute_UnHighlight(Highlightable);
		}
	}

}

void AInv_PlayController::PrimaryInteract()
{
	UE_LOG(LogTemp, Warning, TEXT("PrimaryInteract"));
}

void AInv_PlayController::CreateHUDWidget()
{
	if (!IsLocalController()) return;
	HUDWidget = CreateWidget<UInv_HUDWidget>(this,HUDWidgetClass);

	if (IsValid(HUDWidget))
	{
		HUDWidget->AddToViewport();
	}
}
