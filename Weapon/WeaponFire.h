// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/WeaponBase.h"
#include "WeaponFire.generated.h"

/**
 * 
 */
UCLASS()
class FPS_TESTGAME_API AWeaponFire : public AWeaponBase
{
	GENERATED_BODY()
public:
	AWeaponFire();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	
};
