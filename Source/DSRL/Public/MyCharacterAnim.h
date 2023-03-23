// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DSRL.h"
#include "Animation/AnimInstance.h"
#include "MyCharacterAnim.generated.h"

/**
 * 
 */
UCLASS()
class DSRL_API UMyCharacterAnim : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Player)
	float Speed = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Player)
	float Direction = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Player)
	bool IsInAir = false;
};
	