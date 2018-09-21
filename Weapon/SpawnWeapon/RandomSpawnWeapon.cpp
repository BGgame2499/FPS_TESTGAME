// Fill out your copyright notice in the Description page of Project Settings.

#include "RandomSpawnWeapon.h"
#include "GameMode/FPS_TESTGAMEGameModeBase.h"
#include "Weapon/WeaponBase.h"
#include "Weapon/SpawnWeapon/SpawnWeapon.h"
#include "Engine.h"


// Sets default values
ARandomSpawnWeapon::ARandomSpawnWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	IsRandom = false;
}

// Called when the game starts or when spawned
void ARandomSpawnWeapon::BeginPlay()
{
	Super::BeginPlay();
	CurrentGameMode = Cast<AFPS_TESTGAMEGameModeBase>(GetWorld()->GetAuthGameMode());
	if (!IsRandom && Role == ROLE_Authority)
	{
		SpawnWeaponClass(WeaponClass, GetTransform());
	}


}


// Called every frame
void ARandomSpawnWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

