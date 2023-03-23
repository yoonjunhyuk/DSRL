// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DSRL.h"
#include "GameFramework/Character.h"
#include "MyEnemy.generated.h"

UCLASS()
class DSRL_API AMyEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = FSM)
	class UMyEnemyFSM* FSM;
};
