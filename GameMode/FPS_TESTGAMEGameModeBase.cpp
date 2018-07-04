// Fill out your copyright notice in the Description page of Project Settings.

#include "FPS_TESTGAMEGameModeBase.h"
#include "Weapon/SpawnWeapon/SpawnWeapon.h"
#include "Engine.h"

AFPS_TESTGAMEGameModeBase::AFPS_TESTGAMEGameModeBase()
{
}
void AFPS_TESTGAMEGameModeBase::BeginPlay()
{
	Super::BeginPlay();
}

FString AFPS_TESTGAMEGameModeBase::InitNewPlayer(APlayerController * NewPlayerController, const FUniqueNetIdRepl & UniqueId, const FString & Options, const FString & Portal)
{
	FString Result = Super::InitNewPlayer(NewPlayerController, UniqueId, Options, Portal);

	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, "NewPlayerJoin",true);

	return Result;
}


