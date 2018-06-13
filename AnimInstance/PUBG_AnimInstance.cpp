// Fill out your copyright notice in the Description page of Project Settings.

#include "PUBG_AnimInstance.h"

void UPUBG_AnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	APawn * Pawn = TryGetPawnOwner();
	if (Pawn)
	{
		FVector Velocity = Pawn->GetVelocity();
		FRotator ActorRotation = Pawn->GetActorRotation();	//得到当前ActorRotation
		FRotator ControlRotation = Pawn->GetControlRotation();	//得到当前Pawn的ControlRotation

		//计算混合空间
		Speed = Velocity.Size();
		Direction = CalculateDirection(Velocity, Pawn->GetActorRotation());	//计算速度向量和旋转量之间的夹角

		//计算瞄准偏移
		FRotator DeltaRotation = ControlRotation - ActorRotation;
		Yaw = FRotator::NormalizeAxis(DeltaRotation.Yaw);
		Pitch = FRotator::NormalizeAxis(DeltaRotation.Pitch);

	}

}



