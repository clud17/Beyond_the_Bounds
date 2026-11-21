// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemData.h"
#include "ItemDefinition.generated.h"
/**
 *
 */
UCLASS(BlueprintType, Blueprintable)
class BEYOND_THE_BOUNDS_API UItemDefinition : public UDataAsset
{
	GENERATED_BODY()

public:
	// ItemData 구조체와 동일한 멤버 변수를 정의하여, DataTable에서 아이템 정보를 가져올 수 있도록 함
	// 만약 추가나 제거가 필요하다면, ItemData 구조체와 동일하게 수정해야 함
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
	// 스태틱 메시 참조
	UPROPERTY(EditAnywhere, Category = "Item Data")
	TSoftObjectPtr<UStaticMesh> WorldMesh;
};


