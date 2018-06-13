// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerControllerBase.h"
#include "PlayerCharacter/PlayerCharacterBase.h"
#include "Components/InputComponent.h"
#include "Engine.h"

APlayerControllerBase::APlayerControllerBase()
{
	//PrimaryActorTick.bCanEverTick = true;
	isIgnoreInput = false;
	/*PlayerInputComp = CreateDefaultSubobject<UInputComponent>(TEXT("PlayerInputComp"));
	PlayerInputComp->BindAction("Fire", IE_Pressed, this, &APlayerControllerBase::AttackOnEvent);
	PlayerInputComp->BindAction("Fire", IE_Released, this, &APlayerControllerBase::AttackOffEvent);*/
	
}

void APlayerControllerBase::BeginPlay()
{
	Super::BeginPlay();

	PlayPawn = Cast<APlayerCharacterBase>(GetWorld()->GetFirstPlayerController()->GetPawn());	//获取到当前PlayPawn
	SetIsIgnoreInput(isIgnoreInput);
}

void APlayerControllerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	

}

void APlayerControllerBase::SetIsIgnoreInput(bool isInput)
{
	isIgnoreInput = isInput;

	SetIgnoreLookInput(isIgnoreInput);
	SetIgnoreMoveInput(isIgnoreInput);
}

void APlayerControllerBase::AttackOnEvent()
{
	if (!isIgnoreInput)
	{
		PlayPawn->AttackOn();
	}
}

void APlayerControllerBase::AttackOffEvent()
{
	if (!isIgnoreInput)
	{
		PlayPawn->AttackOff();
	}
}
