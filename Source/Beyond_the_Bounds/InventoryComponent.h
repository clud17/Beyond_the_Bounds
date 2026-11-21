// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ItemDefinition.h"
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

	// Character에서 interact 시 호출되는 아이템 추가 함수
	// Quantity는 해당 액터에서 관리 
	void AddItem(UItemDefinition* Item, int32 Quantity);

	// InventoryComponent에서 사용할 아이템 찾기 함수
	FInventoryItem* FindItem(const UItemDefinition* Item);

	// 인벤토리 내부를 ID 순서대로 정렬
	void SortInventoryByID();

};
