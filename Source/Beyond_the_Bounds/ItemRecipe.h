#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ItemRecipe.generated.h"

class UItemDefinition;

USTRUCT()
struct FRecipeIngredient
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Recipe")
	TObjectPtr<UItemDefinition> items;
	// 현재 인벤토리에 있는 아이템 수량
	UPROPERTY(EditAnywhere, Category = "Recipe")
	int32 quantity;
};

// 아이템의 조합법 데이터 테이블 행을 정의하는 구조체
USTRUCT()
struct FItemRecipe : public FTableRowBase
{
	GENERATED_BODY()

	// RecipeID
	UPROPERTY(EditAnywhere, Category = "Recipe")
	int32 RecipeID;
	// Ingredients
	UPROPERTY(EditAnywhere, Category = "Recipe")
	TArray<FRecipeIngredient> ingredients;
	// Result Item
	UPROPERTY(EditAnywhere, Category = "Recipe")
	TObjectPtr<UItemDefinition> item;
	// Result Items Qunatity
	UPROPERTY(EditAnywhere, Category = "Recipe")
	int32 ResultQuantity;
};