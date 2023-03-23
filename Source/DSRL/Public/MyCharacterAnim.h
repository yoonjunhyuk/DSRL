// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DSRL.h"
#include "Animation/AnimInstance.h"
#include "MyCharacterAnim.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnNextAttackCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnAttackHitCheckDelegate);

/**
 * 
 */
UCLASS()
class DSRL_API UMyCharacterAnim : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UMyCharacterAnim();

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Player)
	float Speed = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Player)
	float Direction = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Player)
	bool IsInAir = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Player)
	UAnimMontage* AttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Player)
	bool IsDead;

	void PlayDeflectAnim();
	void PlayAttackMontage();
	void JumpToAttackMontageSection(int32 NewSection);
	void SetDeadAnim() { IsDead = true; }

	FOnNextAttackCheckDelegate OnNextAttackCheck;
	FOnAttackHitCheckDelegate OnAttackHitCheck;

	UFUNCTION()
	void AnimNotify_AttackHitCheck();

	UFUNCTION()
	void AnimNotify_NextAttackCheck();

	FName GetAttackMontageSectionName(int32 Section);
};