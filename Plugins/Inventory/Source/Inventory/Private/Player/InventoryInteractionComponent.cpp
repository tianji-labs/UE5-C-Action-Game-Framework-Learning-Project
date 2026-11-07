


#include "Player/InventoryInteractionComponent.h"
//计算
#include"Kismet/GameplayStatics.h"
//高亮
#include "Interaction/Inv_Highlightable.h"
//HUD
#include "HUD/Widget/Inv_HUDWidget.h"
//信息
#include "Item/Component/Inv_ItemComponent.h"
//增强输入
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"


UInventoryInteractionComponent::UInventoryInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	TraceLength = 1000.0;

	ItemTraceChannel = ECC_GameTraceChannel1;
}

void UInventoryInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	TraceForItem();

}

void UInventoryInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = Cast<APlayerController>(GetOwner());

	if (!IsValid(PlayerController)) return;
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
	if(IsValid(Subsystem))
	{
		Subsystem->AddMappingContext(IMC_Inventory, 0);
	}

	CreateHUDWidget();

}

void UInventoryInteractionComponent::TraceForItem()
{
	//获得屏幕中心位置
	if (!IsValid(GEngine) || !IsValid(GEngine->GameViewport)) return;
	FVector2D ViewportSize;
	GEngine->GameViewport->GetViewportSize(ViewportSize);
	const FVector2D ViewportCenter = ViewportSize / 2.f;

	//计算相机位置及单位向量
	FVector TraceStart;
	FVector Forward;
	if (!UGameplayStatics::DeprojectScreenToWorld(PlayerController, ViewportCenter, TraceStart, Forward)) return;
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

void UInventoryInteractionComponent::CreateHUDWidget()
{
	HUDWidget = CreateWidget<UInv_HUDWidget>(PlayerController, HUDWidgetClass);

	if (IsValid(HUDWidget))
	{
		HUDWidget->AddToViewport();
	}
}

void UInventoryInteractionComponent::BindInput(UEnhancedInputComponent* EnhancedInputComponent)
{
	EnhancedInputComponent->BindAction(PrimaryInteractAction, ETriggerEvent::Started, this, &UInventoryInteractionComponent::PrimaryInteract);
}

void UInventoryInteractionComponent::PrimaryInteract()
{
	UE_LOG(LogTemp, Warning, TEXT("PrimaryInteract"));
}