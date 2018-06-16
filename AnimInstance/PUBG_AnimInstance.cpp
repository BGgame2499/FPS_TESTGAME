// Fill out your copyright notice in the Description page of Project Settings.

#include "PUBG_AnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PlayerCharacter/PlayerCharacterBase.h"

void UPUBG_AnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	CurrentPlayerPawn = Cast<APlayerCharacterBase>(TryGetPawnOwner());
	if (CurrentPlayerPawn)
	{
		FVector Velocity = CurrentPlayerPawn->GetVelocity();
		FRotator ActorRotation = CurrentPlayerPawn->GetActorRotation();	//得到当前ActorRotation
		FRotator ControlRotation = CurrentPlayerPawn->GetControlRotation();	//得到当前Pawn的ControlRotation

		//计算混合空间
		Speed = Velocity.Size();
		Direction = CalculateDirection(Velocity, CurrentPlayerPawn->GetActorRotation());	//计算速度向量和旋转量之间的夹角

		//计算瞄准偏移
		FRotator DeltaRotation = ControlRotation - ActorRotation;
		Yaw = FRotator::NormalizeAxis(DeltaRotation.Yaw);
		Pitch = FRotator::NormalizeAxis(DeltaRotation.Pitch) * 1.5f;

		//检测跳跃
		IsJump = CurrentPlayerPawn->GetMovementComponent()->IsFalling();

		//检测下蹲
		IsCrouch = CurrentPlayerPawn->GetMovementComponent()->IsCrouching();

	}

}

void UPUBG_AnimInstance::AnimNotify_TakeWeaponGun(UAnimNotify * Notify)
{
	if (CurrentPlayerPawn)
	{
		CurrentPlayerPawn->UpdateWeapon();
	}
}

void UPUBG_AnimInstance::AnimNotify_DownWeaponGun(UAnimNotify * Notify)
{
	if (CurrentPlayerPawn)
	{
		CurrentPlayerPawn->UpdateWeapon();
	}
}
