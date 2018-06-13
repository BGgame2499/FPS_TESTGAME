// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/WeaponBase.h"
#include "WeaponGun.generated.h"

/**
 * 
 */
UCLASS()
class FPS_TESTGAME_API AWeaponGun : public AWeaponBase
{
	GENERATED_BODY()
public:
	AWeaponGun();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USkeletalMeshComponent * WeaponSkletalMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "WeponeBasicValue")
	FName CurrentMuzzleName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeponeBasicValue")
	FVector CurrentMuzzleLocation;

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual bool OnAttack()override;
	virtual bool OffAttack()override;
	
};
