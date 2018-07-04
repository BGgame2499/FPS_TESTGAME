// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnWeapon.generated.h"

/**
 * 
 */
UCLASS()
class FPS_TESTGAME_API ASpawnWeapon : public AActor
{
	GENERATED_BODY()

public:
	ASpawnWeapon();
	UFUNCTION(BlueprintCallable)
		class AWeaponBase * SpawnWeaponClass(TSubclassOf<AWeaponBase > WeaponClass, FTransform WeaponLocation);
	UFUNCTION(BlueprintCallable)
	 class AWeaponBase * SpawnWeaponClassAndBullet(TSubclassOf<AWeaponBase > WeaponClass, FTransform WeaponLocation,int32 CurrentBullet,int32 ReserveBullet);
	
};
