
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
			// 아래는 return이 아니라 바닥에 떨궈야함.
			return;
		}

		ExistingItem->quantity += Quantity;

		return;
	}

	InventoryItems.Add(FInventoryItem{ Item, Quantity });

	SortInventoryByID();
}

// 아이템 제거 함수
void UInventoryComponent::RemoveItem(UItemDefinition* Item, int32 Quantity) 
{
	if (!Item || Quantity <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid item or quantity"));
		return;
	}
	
	FInventoryItem* ExistingItem = FindItem(Item);
	if (ExistingItem)
	{
		if (ExistingItem->quantity >= Quantity) // 현재 가지고 있는 아이템 수량이 요구하는 아이템보다 많을 때
		{
			ExistingItem->quantity -= Quantity;

			// 아이템 수량이 0 이하면 인벤토리에서 삭제
			InventoryItems.RemoveAll([](const FInventoryItem& Invenitem)
				{
					return Invenitem.quantity <= 0;
				});
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("You dont have amount of quantity"));
		}
	}
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

// 아이템 조합 함수
void UInventoryComponent::CraftItem(FName RecipeName)
{
	// 서버 권위 검사
	if (!GetOwner() || !GetOwner()->HasAuthority())
	{
		return;
	}

	if (!CraftingTable) return;

	// FindRow를 통해 조합법을 찾아서, 이 조합법에 맞는 크래프팅 실행 
	FItemRecipe* ItemRecipe;
	ItemRecipe = CraftingTable->FindRow<FItemRecipe>(RecipeName, TEXT("CraftItem"));

	// 조합이 가능한지 확인하는 변수
	bool canCraft;
	canCraft = AvailableCraft(RecipeName);

	// 수량이 부족하지 않고 조합이 가능할 때, 재료 삭제 후 아이템 추가
	if (ItemRecipe && canCraft) 
	{
		// 조합법에 있는 재료들을 순회하면서 조합 실행
		for (int i = 0; i < ItemRecipe->ingredients.Num(); i++) 
		{
			RemoveItem(ItemRecipe->ingredients[i].items, ItemRecipe->ingredients[i].quantity);
		}
		
		AddItem(ItemRecipe->item, ItemRecipe->ResultQuantity);
	}
}

bool UInventoryComponent::AvailableCraft(FName RecipeName)
{
	if (!CraftingTable) return false;

	FItemRecipe* ItemRecipe;
	ItemRecipe = CraftingTable->FindRow<FItemRecipe>(RecipeName, TEXT("CraftItem"));

	// 조합법을 찾을 수 없으면 fasle
	if (!ItemRecipe) return false;
	
	// 현재 조합이 가능한지 확인하는 리턴 값
	bool availableCraft = false;

	// 재료의 아이템과 현재 인벤토리 아이템의 수량을 비교해서 
	for (int i = 0; i < ItemRecipe->ingredients.Num(); i++) 
	{
		const FInventoryItem* ExistingItem = FindItem(ItemRecipe->ingredients[i].items);

		// 조합법에 없는 아이템을 찾으면 false
		if (!ExistingItem) return false;

		if (ExistingItem->quantity >= ItemRecipe->ingredients[i].quantity)
		{
			availableCraft = true;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("You dont have amount of quantity, so you cant craft"));
			availableCraft = false;
			break;
		}
	}

	return availableCraft;
}