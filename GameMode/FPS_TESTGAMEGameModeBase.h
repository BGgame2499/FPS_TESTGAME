// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "FPS_TESTGAMEGameModeBase.generated.h"

enum class EWaveState : uint8;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnActorKilled, AActor*, VictimActor, AActor*, KillerActor,AController*,KillerController);

/**
 * 
 */
UCLASS()
class FPS_TESTGAME_API AFPS_TESTGAMEGameModeBase : public AGameMode
{
	GENERATED_BODY()

		AFPS_TESTGAMEGameModeBase();

public:
	virtual void BeginPlay();

	void CompleteMission(APawn * InstigatorPawn);

protected:

	FTimerHandle TimeHandle_BotSpawner;

	FTimerHandle TimeHandle_NextWaveStart;

	int32 NrOfBotsToSpawn;	//当前波要生成的敌人
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Wave")
		int32 WaveCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
		int32 ZombieCount;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category = "GameMode")
	float TimeBetweenWave;

protected:
	/* Called once on every new player that enters the gamemode 对每一个进入游戏玩家都调用一次*/
	virtual FString InitNewPlayer(class APlayerController* NewPlayerController, const FUniqueNetIdRepl& UniqueId, const FString& Options, const FString& Portal = TEXT("")) override;
	
	UFUNCTION(BlueprintImplementableEvent,Category = "GameMode")
	void SpawnNewBot();

	void SpawnBotTimerElapsed();

	void StartWave();

	void EndWave();

	void PrepareForNextWave();

	void CheckWaveState();

	void CheckAnyPlayer();

	UFUNCTION(BlueprintCallable)
	void GameOver();

	void SetWaveState(EWaveState NewState);

	void RestarDeadPlayers();

	virtual void Tick(float DeltaSeconds)override;

public:


	virtual void StartPlay()override;

	UPROPERTY(BlueprintAssignable,Category = "GameMode")
	FOnActorKilled OnActorKilled;

};
