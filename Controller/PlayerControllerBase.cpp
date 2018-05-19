// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerControllerBase.h"
#include "PlayerCharacter/PlayerCharacterBase.h"
#include "Engine.h"

void APlayerControllerBase::BeginPlay()
{
	Super::BeginPlay();

	PlayPawn = Cast<APlayerCharacterBase>(GetWorld()->GetFirstPlayerController()->GetPawn());	//获取到当前PlayPawn
	
}

void APlayerControllerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}
