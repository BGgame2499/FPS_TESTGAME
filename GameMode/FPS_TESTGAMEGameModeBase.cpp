// Fill out your copyright notice in the Description page of Project Settings.

#include "FPS_TESTGAMEGameModeBase.h"
#include "Weapon/SpawnWeapon/SpawnWeapon.h"
#include "TimerManager.h"
#include "Components/HealthComponent.h"
#include "Engine.h"

AFPS_TESTGAMEGameModeBase::AFPS_TESTGAMEGameModeBase()
{
	TimeBetweenWave = 5.0f;

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 1.0f;

}
void AFPS_TESTGAMEGameModeBase::BeginPlay()
{
	Super::BeginPlay();


}

void AFPS_TESTGAMEGameModeBase::Tick(float DeltaSeconds)	//每秒更新
{
	Super::Tick(DeltaSeconds);

	CheckWaveState();
}

void AFPS_TESTGAMEGameModeBase::CompleteMission(APawn * InstigatorPawn)
{
	if (InstigatorPawn)
	{
		InstigatorPawn->DisableInput(nullptr);
	}
	//UGameInstance* MyInstance = GetGameState<AFPS_TESTGAMEGameModeBase>()->GetGameInstance();
	//GetGameInstance();
	
}

FString AFPS_TESTGAMEGameModeBase::InitNewPlayer(APlayerController * NewPlayerController, const FUniqueNetIdRepl & UniqueId, const FString & Options, const FString & Portal)
{
	FString Result = Super::InitNewPlayer(NewPlayerController, UniqueId, Options, Portal);

	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, "NewPlayerJoin",true);

	return Result;
}


void AFPS_TESTGAMEGameModeBase::StartPlay()
{
	Super::StartPlay();

	PrepareForNextWave();

}
void AFPS_TESTGAMEGameModeBase::PrepareForNextWave()
{

	GetWorldTimerManager().SetTimer(TimeHandle_NextWaveStart, this, &AFPS_TESTGAMEGameModeBase::StartWave, TimeBetweenWave, false);	//延时调用波生成
}

void AFPS_TESTGAMEGameModeBase::CheckWaveState()
{
	bool bIsPrepareingForWave = GetWorldTimerManager().IsTimerActive(TimeHandle_NextWaveStart);

	if (NrOfBotsToSpawn > 0 || bIsPrepareingForWave)
	{
		return;
	}

	bool bIsAnyBotAlive = false;

	for (FConstPawnIterator it = GetWorld()->GetPawnIterator();it ; ++it)
	{
		APawn * TestPawn = it->Get();
		if (TestPawn == nullptr || TestPawn->IsPlayerControlled())
		{
			continue;
		}
		UHealthComponent * HealthComp = Cast<UHealthComponent>(TestPawn->GetComponentByClass(UHealthComponent::StaticClass()));
		if (HealthComp && HealthComp->GetHealth() > 0.0f)
		{
			bIsAnyBotAlive = true;
			break;
		}
	}

	if (!bIsAnyBotAlive)
	{
		PrepareForNextWave();
	}

}

void AFPS_TESTGAMEGameModeBase::CheckAnyPlayer()	//检查所有玩家是否存活
{

	for (FConstPlayerControllerIterator it = GetWorld()->GetPlayerControllerIterator(); it; ++it)
	{
		APlayerController * PC = it->Get();
		if (PC && PC->GetPawn())
		{
			APawn * MyPawn = PC->GetPawn();
			UHealthComponent * HealthComp = Cast<UHealthComponent>(MyPawn->GetComponentByClass(UHealthComponent::StaticClass()));

			if (ensure(HealthComp) && HealthComp->GetHealth() > 0.0f)	//保证玩家拥有血量组件
			{
				return;
			}
			
		}
	}

	//无玩家存活


	GameOver();

}

void AFPS_TESTGAMEGameModeBase::GameOver()
{
	EndWave();

	UE_LOG(LogTemp, Log, TEXT("GameOver!!!"));
}

void AFPS_TESTGAMEGameModeBase::StartWave()
{
	WaveCount++;

	NrOfBotsToSpawn = 2 * WaveCount;


	GetWorldTimerManager().SetTimer(TimeHandle_BotSpawner,this,&AFPS_TESTGAMEGameModeBase::SpawnBotTimerElapsed,1.0f ,true , 0.0f);
}
void AFPS_TESTGAMEGameModeBase::SpawnBotTimerElapsed()
{
	SpawnNewBot();

	NrOfBotsToSpawn--;

	if (NrOfBotsToSpawn <= 0)
	{
		EndWave();
	}


}
void AFPS_TESTGAMEGameModeBase::EndWave()
{

	GetWorldTimerManager().ClearTimer(TimeHandle_BotSpawner);
	CheckWaveState();
}

