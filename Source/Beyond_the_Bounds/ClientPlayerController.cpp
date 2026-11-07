#include "ClientPlayerController.h"
#include "EnhancedInputSubsystems.h"

AClientPlayerController::AClientPlayerController()
	:
	InputMappingContext(nullptr),
	MoveAction(nullptr),
	JumpAction(nullptr),
	LookAction(nullptr),
	InteractAction(nullptr),
	AttackAction(nullptr)
{
}

void AClientPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	if (ULocalPlayer* LocalPlayer = GetLocalPlayer()) 
	{
		if (UEnhancedInputLocalPlayerSubsystem* SubSystem =
			LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>()) 
		{
			if (InputMappingContext) 
			{
				SubSystem->AddMappingContext(InputMappingContext, 0);
			}
		}
	}
}