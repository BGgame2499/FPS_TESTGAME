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

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PUBG")
		bool isTurnLeft;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PUBG")
		bool isTurnRight;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PUBG")
		bool isPunch;

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterBase")
		class APlayerCharacterBase * CurrentPlayerPawn;
	
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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PUBG")
		bool IsAim;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PUBG")
		bool isWeaponAttackFire; 
	

public:
	UFUNCTION(BlueprintCallable)
	void AnimNotify_TakeWeaponGun(UAnimNotify * Notify);

	UFUNCTION(BlueprintCallable)
	void AnimNotify_DownWeaponGun(UAnimNotify * Notify);
};
