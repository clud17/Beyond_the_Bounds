// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Net/UnrealNetwork.h"
#include "ClientPlayerController.h"
#include "EnhancedInputComponent.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void APlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APlayerCharacter, InteractCount);
}


// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	//UE_LOG(LogTemp, Warning, TEXT("GetName: %s"), *GetName());
	//UE_LOG(LogTemp, Warning, TEXT("IsLocallyControlled: %s"), IsLocallyControlled() ? TEXT("true") : TEXT("false"));
	//UE_LOG(LogTemp, Warning, TEXT("LocalRole: %s"), *UEnum::GetValueAsString(GetLocalRole()));
	//UE_LOG(LogTemp, Warning, TEXT("GetOwner: %s"), GetOwner() ? *GetOwner()->GetName() : TEXT("None"));
	//UE_LOG(LogTemp, Warning, TEXT("GetNetConnection: %s"), GetNetConnection() ? *GetNetConnection()->GetName() : TEXT("None"));
	//UE_LOG(LogTemp, Warning, TEXT("Authority: %s"), HasAuthority() ? TEXT("true") : TEXT("false"));
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (AClientPlayerController* PlayerController = Cast<AClientPlayerController>(GetController())) 
		{
			if (PlayerController->MoveAction) 
			{
				EnhancedInput->BindAction(PlayerController->MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
			}
			if (PlayerController->JumpAction)
			{
				EnhancedInput->BindAction(PlayerController->JumpAction, ETriggerEvent::Triggered, this, &APlayerCharacter::StartJump);
				EnhancedInput->BindAction(PlayerController->JumpAction, ETriggerEvent::Completed, this, &APlayerCharacter::StopJump);
			}
			if (PlayerController->LookAction) 
			{
				EnhancedInput->BindAction(PlayerController->LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
			}
			if (PlayerController->InteractAction)
			{
				EnhancedInput->BindAction(PlayerController->InteractAction, ETriggerEvent::Started, this, &APlayerCharacter::Interact);
			}
		}
	}
}

void APlayerCharacter::Move(const FInputActionValue& value)
{
	// 컨트롤러가 있어야 방향 계산 가능
	if (!Controller) return;

	// Value는 Axis2D로 설정된 IA_Move의 입력값을 가져옴
	const FVector2D MoveInput = value.Get<FVector2D>();

	if (!FMath::IsNearlyZero(MoveInput.X))
	{
		// 캐릭터가 바라보는 방향(정면)으로 X축 이동
		AddMovementInput(GetActorForwardVector(), MoveInput.X * moveSpeed);
	}

	if (!FMath::IsNearlyZero(MoveInput.Y))
	{
		// 캐릭터의 오른쪽 방향으로 Y축 이동
		AddMovementInput(GetActorRightVector(), MoveInput.Y * moveSpeed);
	}
}

void APlayerCharacter::StartJump(const FInputActionValue& value)
{
	// Jump 함수는 Character가 기본 제공
	if (value.Get<bool>())
	{
		Jump();
	}
}

void APlayerCharacter::StopJump(const FInputActionValue& value)
{
	// StopJumping 함수도 Character가 기본 제공
	if (value.Get<bool>())
	{
		StopJumping();
	}
}

void APlayerCharacter::Look(const FInputActionValue& value)
{
	// 마우스의 X, Y 움직임을 2D 축으로 가져옴
	FVector2D LookInput = value.Get<FVector2D>();

	// x는 좌우 회전, y는 상하 회전
	// 좌우 회전
	AddControllerYawInput(LookInput.X);
	// 상하 회전
	AddControllerPitchInput(LookInput.Y);
}

void APlayerCharacter::Interact(const FInputActionValue& value)
{
	// 상호작용 로직 구현
	if (value.Get<bool>())
	{
		Server_Interact();
	}
}

void APlayerCharacter::Server_Interact_Implementation()
{
	// 서버에서 상호작용 로직 구현
	if (HasAuthority()) 
	{
		//UE_LOG(LogTemp, Warning, TEXT("Server Interact action triggered"));
		InteractCount++;
		UE_LOG(LogTemp, Warning, TEXT("Interaction count: %d"), InteractCount);
	}	
}

void APlayerCharacter::OnRep_InteractionCount()
{
	UE_LOG(LogTemp, Warning, TEXT("OnRep: Interaction count updated: %d"), InteractCount);
}