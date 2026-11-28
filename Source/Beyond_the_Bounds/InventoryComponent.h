// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ItemDefinition.h"
#include "ItemRecipe.h"
#include "InventoryComponent.generated.h"

// 인벤토리에서 쓸 아이템 구조체 정의
USTRUCT(BlueprintType)
struct FInventoryItem
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Inventory")
	TObjectPtr<UItemDefinition> items;
	// 현재 인벤토리에 있는 아이템 수량
	UPROPERTY(EditAnywhere, Category = "Inventory")
	int32 quantity;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BEYOND_THE_BOUNDS_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	UPROPERTY(ReplicatedUsing = OnRep_InventoryItems, VisibleAnywhere ,BlueprintReadOnly, Category = "Inventory")
	// 인벤토리에서 사용할 아이템 배열
	TArray<FInventoryItem> InventoryItems;

	// 리플리케이션 OnRep 함수
	UFUNCTION()
	void OnRep_InventoryItems();

	// AddItem이랑 RemoveItem을 호출할 때 HasAutority()를 통해 서버 권위 검사 해야함!
	// PlayerCharacter에서 검사 진행

	// Character에서 interact 시 호출되는 아이템 추가 함수
	// Quantity는 해당 액터에서 관리 
	void AddItem(UItemDefinition* Item, int32 Quantity);

	// 아이템 제거 함수
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void RemoveItem(UItemDefinition* Item, int32 Quantity);

	// InventoryComponent에서 사용할 아이템 찾기 함수
	FInventoryItem* FindItem(const UItemDefinition* Item);

	// 인벤토리 내부를 ID 순서대로 정렬
	void SortInventoryByID();

	// 아이템 조합 함수 // 조합법 이름를 받아와서 이에 맞는 조합 실행
	void CraftItem(FName RecipeName);
	// 조합 가능 확인(갯수 확인) 함수
	bool AvailableCraft(const FName RecipeName);
	// 조합법 DataTable
	UPROPERTY(EditAnywhere, Category = "Crafting")
	TObjectPtr<UDataTable> CraftingTable;
};
