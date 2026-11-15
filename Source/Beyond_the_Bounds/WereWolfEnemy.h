// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyBase.h"
#include "WereWolfEnemy.generated.h"

/**
 * 
 */
UCLASS()
class BEYOND_THE_BOUNDS_API AWereWolfEnemy : public AEnemyBase
{
	GENERATED_BODY()
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
