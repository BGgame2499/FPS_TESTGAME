// Fill out your copyright notice in the Description page of Project Settings.
#include "Struct/BasicStruct.h"
#include "Components/SkeletalMeshComponent.h"
#include "Weapon/WeaponBase.h"
	
FTrench::FTrench()
{
	TrenchName = "NULL";
	Weapon = nullptr;

}

AWeaponBase * FTrench::GetWeapon()
{
	if (Weapon)
	{
		AWeaponBase * weapon = Weapon;
		Weapon = nullptr;
		return weapon;
	}
	return nullptr;
}

bool FTrench::SetWeapon(USkeletalMeshComponent * mesh, AWeaponBase * weapon)
{
	if (TrenchName == "NULL" || weapon == nullptr)
	{
		return false;
	}
	weapon->TrenchName = TrenchName;
	Weapon = weapon;
	Weapon->AttachToComponent(mesh, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TrenchName);
	return true;
}