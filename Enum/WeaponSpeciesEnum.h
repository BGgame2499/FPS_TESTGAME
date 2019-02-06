// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponSpeciesEnum.generated.h"

UENUM(Blueprintable, BlueprintType)
enum class WeaponSpeciesEnum : uint8
{
	//E_SCAN UMETA(DisplayName = "SCAN"),
	SK_S96,
	SK_KA74U,
	SK_SMG45,
	SK_PISTOL_1,
	SK_AWP,
	SK_BOMBLAUNC,
	FIST
	
};
