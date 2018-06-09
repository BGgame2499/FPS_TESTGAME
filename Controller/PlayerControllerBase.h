// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayerControllerBase.generated.h"

/**
 * 
 */
UCLASS()
class FPS_TESTGAME_API APlayerControllerBase : public APlayerController
{
	GENERATED_BODY()

		APlayerControllerBase();
public:
	UPROPERTY(BlueprintReadWrite, Category = "PlayPawn")
		class APlayerCharacterBase * PlayPawn;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Input")
		bool isIgnoreInput;

	//class UInputComponent * PlayerInputComp;

public:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
		void SetIsIgnoreInput(bool isInput);
	
	UFUNCTION(BlueprintCallable)
	virtual void AttackOnEvent();
	UFUNCTION(BlueprintCallable)
	virtual void AttackOffEvent();


};
