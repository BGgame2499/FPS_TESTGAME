// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerStateEnum.generated.h"

UENUM(Blueprintable, BlueprintType)
enum class PlayerStateEnum : uint8
{
	//E_SCAN UMETA(DisplayName = "SCAN"),
	Idle,
	Walk,
	Run,
	Sprint,
	Aim,

	Crouch_Idle,
	Crouch_Walk,
	Crouch_Run,
	Crouch_Aim
	
};
