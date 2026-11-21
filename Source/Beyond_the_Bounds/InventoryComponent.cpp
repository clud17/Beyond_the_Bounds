
#include "InventoryComponent.h"
#include "Net/UnrealNetwork.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	// Replication 활성화
	SetIsReplicatedByDefault(true);
}
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

}
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UInventoryComponent, InventoryItems);
}

void UInventoryComponent::OnRep_InventoryItems() 
{
	// Replication된 InventoryItems 값이 변경될 때 호출되는 함수
	UE_LOG(LogTemp, Warning, TEXT("OnRep: InventoryItems updated"));
}

// Character에서 interact 시 호출되는 아이템 추가 함수
void UInventoryComponent::AddItem(UItemDefinition* Item, int32 Quantity)
{
	if (!Item || Quantity <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid item or quantity"));
		return;
	}
	// **** 나중에 차이만큼 바닥에 떨구는거 구현해야함 ****
	if (Quantity > Item->MaxStack) return;

	// 이미 인벤토리에 존재하는 아이템인지 확인
	// 주소를 반환하기 때문에 FindItem 함수에서 반환된 포인터를 사용하여 수량을 증가시킬 수 있음
	FInventoryItem* ExistingItem = FindItem(Item);
	if (ExistingItem)
	{
		// 수량이 최대 중첩 갯수를 초과하지 않도록 확인
		if (ExistingItem->quantity + Quantity > ExistingItem->items->MaxStack)
		{
			UE_LOG(LogTemp, Warning, TEXT("Cannot add item: Exceeds max stack limit"));
			// **** 나중에 차이만큼 바닥에 떨구는거 구현해야함 ****
			return;
		}

		ExistingItem->quantity += Quantity;

		return;
	}

	InventoryItems.Add(FInventoryItem{ Item, Quantity });

	SortInventoryByID();
}

// InventoryComponent에서 사용할 아이템 찾기 함수 (ID 기반 Find)
FInventoryItem* UInventoryComponent::FindItem(const UItemDefinition* Item)
{
	// 유효성 검사
	if (!Item) return nullptr;

	for (int i = 0; i < InventoryItems.Num(); i++) {
		if (InventoryItems[i].items)  // 유효성 검사
		{
			if (InventoryItems[i].items->ID == Item->ID) {
				return &InventoryItems[i]; // 아이템의 주소 반환
			}
		}
	}
	return nullptr;
}

// 인벤토리 내부를 ID 순서대로 정렬
void UInventoryComponent::SortInventoryByID()
{
	InventoryItems.Sort([](const FInventoryItem& A, const FInventoryItem& B)
		{	return A.items->ID < B.items->ID;}
	);
}
