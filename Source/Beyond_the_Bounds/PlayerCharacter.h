// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

struct FInputActionValue;

UCLASS()
class BEYOND_THE_BOUNDS_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	// input binding functions
	UFUNCTION()
	void Move(const FInputActionValue& Value);
	UFUNCTION()
	void StartJump(const FInputActionValue& Value);
	UFUNCTION()
	void StopJump(const FInputActionValue& Value);
	UFUNCTION()
	void Look(const FInputActionValue& Value);
	UFUNCTION()
	void Interact(const FInputActionValue& Value);
	// Server RPC - interaction functions
	UFUNCTION(Server, Reliable)
	void Server_Interact();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
	TObjectPtr<class UHealthComponent> HealthComponent;

private:
	UPROPERTY(EditAnywhere, Category = "Movement")
	float moveSpeed = 500.0f;


public:
	UPROPERTY(ReplicatedUsing = OnRep_InteractionCount)
	int InteractCount = 0;

	UFUNCTION()
	void OnRep_InteractionCount();

	
};
