// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "FPS_TESTGAMEGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class FPS_TESTGAME_API AFPS_TESTGAMEGameModeBase : public AGameMode
{
	GENERATED_BODY()

		AFPS_TESTGAMEGameModeBase();

public:
public:
	virtual void BeginPlay();

	void CompleteMission(APawn * InstigatorPawn);

protected:
	/* Called once on every new player that enters the gamemode 对每一个进入游戏玩家都调用一次*/
	virtual FString InitNewPlayer(class APlayerController* NewPlayerController, const FUniqueNetIdRepl& UniqueId, const FString& Options, const FString& Portal = TEXT("")) override;
	
};
