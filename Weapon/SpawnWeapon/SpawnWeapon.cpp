// Fill out your copyright notice in the Description page of Project Settings.

#include "SpawnWeapon.h"
#include "Weapon/WeaponBase.h"
#include "Engine.h"
ASpawnWeapon::ASpawnWeapon()
{

}
AWeaponBase * ASpawnWeapon::SpawnWeaponClass(TSubclassOf<AWeaponBase > WeaponClass, FTransform WeaponLocation)
{
	AWeaponBase * NewWeapon = GetWorld()->SpawnActor<AWeaponBase>(WeaponClass, WeaponLocation);
	if (NewWeapon)
	{
		return NewWeapon;
	}
		return nullptr;
}

AWeaponBase * ASpawnWeapon::SpawnWeaponClassAndBullet(TSubclassOf<AWeaponBase> WeaponClass, FTransform WeaponLocation, int32 CurrentBullet, int32 ReserveBullet)
{
	AWeaponBase * NewWeapon = GetWorld()->SpawnActor<AWeaponBase>(WeaponClass, WeaponLocation);
	if (NewWeapon)
	{
		NewWeapon->SetWeaponBullet(CurrentBullet, ReserveBullet);
		return NewWeapon;
	}
	return nullptr;
}
