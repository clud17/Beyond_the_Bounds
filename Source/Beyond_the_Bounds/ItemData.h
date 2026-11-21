#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ItemData.generated.h"

class UItemDefinition;

// 아이템의 타입을 정의하는 열거형
UENUM()
enum class EItemType : uint8
{
	Weapon UMETA(DisplayName = "Weapon"), // 무기, 플레이어가 장착하고 공격에 사용
	Armor UMETA(DisplayName = "Armor"), // 방어구, 플레이어가 장착하고 방어에 사용
	Consumable UMETA(DisplayName = "Consumable"), // 소모품, 플레이어가 사용하여 일회성 효과를 얻음
	Material UMETA(DisplayName = "Material"), // 재료, 플레이어가 사용하여 제작에 사용

};

// 아이템의 이름과 설명 등 저장
USTRUCT()
struct FItemText
{
	GENERATED_BODY()

	// 아이템 이름
	UPROPERTY(EditAnywhere)
	FText Name;
	// 아이템 설명
	UPROPERTY(EditAnywhere)
	FText Description;
};

// 아이템의 데이터 테이블 행을 정의하는 구조체
// 컴포지션을 사용하여 아이템 정의(EItemType)와 아이템 텍스트(FItemText)를 포함
USTRUCT()
struct FItemData : public FTableRowBase
{
	GENERATED_BODY()

	// 아이템의 고유 ID
	UPROPERTY(EditAnywhere, Category = "Item Data")
	int32 ID;
	// 아이템의 정의 참조
	UPROPERTY(EditAnywhere, Category = "Item Data")
	FItemText ItemText;
	// 재료, 무기, 방어구 등 구분
	UPROPERTY(EditAnywhere, Category = "Item Data")
	EItemType ItemType;
	// 최대 중첩 갯수
	UPROPERTY(EditAnywhere, Category = "Item Data")
	int32 MaxStack;
	// 인벤토리 아이콘 참조
	UPROPERTY(EditAnywhere, Category = "Item Data")
	TSoftObjectPtr<UTexture2D> Icon;
	// DataTable에 저장된 아이템 정보와 
	// 실제 게임에서 사용할 아이템 정의를 
	// 연결하기 위한 참조 변수
	UPROPERTY(EditAnywhere, Category = "Item Data")
	TObjectPtr<UItemDefinition> ItemBase;
};