// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PUBG_AnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class FPS_TESTGAME_API UPUBG_AnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	void NativeUpdateAnimation(float DeltaSeconds)override;

protected:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PUBG")
		float Speed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PUBG")
		float Direction;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PUBG")
		float Pitch;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PUBG")
		float Yaw;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PUBG")
		bool IsJump;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PUBG")
		bool IsCrouch;
	
};
