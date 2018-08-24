// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/WeaponBase.h"
#include "WeaponBomb.generated.h"

class URadialForceComponent;

UCLASS()
class FPS_TESTGAME_API AWeaponBomb : public AWeaponBase
{
	GENERATED_BODY()
public:
	AWeaponBomb();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RadialForceComp")
		 URadialForceComponent * RadialForceComp;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RadialForce")
		float RadialForceRadius;
protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	
};
