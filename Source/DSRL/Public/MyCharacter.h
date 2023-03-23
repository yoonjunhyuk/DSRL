// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DSRL.h"
#include "GameFramework/Character.h"
#include "MyCharacter.generated.h"

UCLASS()
class DSRL_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PostInitializeComponents() override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

public:
	UPROPERTY(VisibleAnywhere, Category = Camera)
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, Category = Player)
	float WalkSpeed = 400;

	UPROPERTY(EditAnywhere, Category = Player)
	float RunSpeed = 800;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack)
	bool CanNextCombo;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack)
	bool IsComboInputOn;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack)
	int32 CurrentCombo;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack)
	int32 MaxCombo;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack)
	float AttackRange = 200.0f;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack)
	float AttackRadius = 50.0f;

	UPROPERTY()
	class UMyCharacterAnim* MyAnim;

	void Turn(float value);
	void LookUp(float value);
	void InputHorizontal(float value);
	void InputVertical(float value);
	void Move();
	void InputJump();
	void InputRun();
	void InputAttack();

	void AttackStartCombo();
	void AttackEndCombo();
	void AttackCheck();

	FVector Direction;

	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	bool IsAttacking;
};
