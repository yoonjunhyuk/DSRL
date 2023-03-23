// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DSRL.h"
#include "Components/ActorComponent.h"
#include "MyEnemyFSM.generated.h"

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	Idle,
	Move,
	Attack,
	Damage,
	Die,
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DSRL_API UMyEnemyFSM : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMyEnemyFSM();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = FSM)
	EEnemyState State = EEnemyState::Idle;

	UPROPERTY(EditDefaultsOnly, Category = FSM)
	float IdleDelayTime = 1.0f;

	UPROPERTY(EditDefaultsOnly, Category = FSM)
	class AMyCharacter* Target;

	UPROPERTY(EditAnywhere, Category = FSM)
	float AttackRange = 150.0f;

	UPROPERTY(EditAnywhere, Category = FSM)
	float AttackDelay = 2.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = FSM)
	int32 HP = 3.0f;

	UPROPERTY(EditAnywhere, Category = FSM)
	float DamageDelay = 2.0f;

	UPROPERTY()
	class AMyEnemy* Me;

	void IdleState();
	void MoveState();
	void AttackState();
	void DamageState();
	void DieState();
	void OnDamageProcess();

	float CurrentTime = 0;
};
