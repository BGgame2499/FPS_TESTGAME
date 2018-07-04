// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/SpawnWeapon/SpawnWeapon.h"
#include "RandomSpawnWeapon.generated.h"

UCLASS()
class FPS_TESTGAME_API ARandomSpawnWeapon : public ASpawnWeapon
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARandomSpawnWeapon();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class ASpawnWeapon * GameModeWeaponSpawner;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class AFPS_TESTGAMEGameModeBase * CurrentGameMode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class AWeaponBase > WeaponClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsRandom;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
