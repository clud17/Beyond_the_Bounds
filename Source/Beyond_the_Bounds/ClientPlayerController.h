#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ClientPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;

UCLASS()
class BEYOND_THE_BOUNDS_API AClientPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AClientPlayerController();

	// Input Mapping Context and Actions
	UPROPERTY(EditAnyWhere, BluePrintReadWrite, Category = "Input");
	UInputMappingContext* InputMappingContext;
	UPROPERTY(EditAnyWhere, BluePrintReadWrite, Category = "Input");
	UInputAction* MoveAction;
	UPROPERTY(EditAnyWhere, BluePrintReadWrite, Category = "Input");
	UInputAction* JumpAction;
	UPROPERTY(EditAnyWhere, BluePrintReadWrite, Category = "Input");
	UInputAction* LookAction;
	UPROPERTY(EditAnyWhere, BluePrintReadWrite, Category = "Input");
	UInputAction* InteractAction;

protected:
	virtual void BeginPlay() override;

};
