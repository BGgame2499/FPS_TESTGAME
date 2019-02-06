// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/WeaponBase.h"
#include "WeaponBomb.generated.h"

class URadialForceComponent;
class UParticleSystem;
class USoundCue;
class USkeletalMeshComponent;

UCLASS()
class FPS_TESTGAME_API AWeaponBomb : public AWeaponBase
{
	GENERATED_BODY()
public:
	AWeaponBomb();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RadialForceComp")
		URadialForceComponent * RadialForceComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MeshComp")
		USkeletalMeshComponent * MeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particle")
		UParticleSystem * ExplosionEffect;		//爆炸特效
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ParticleSound")
		USoundCue * ExplosionSound;		//爆炸音效

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BombBasicVolue")
		float ExplosinDelayTime;	//延时爆炸时间
protected:
	virtual void BeginPlay() override;

	FTimerHandle ExplosionTimeHandle;

public:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
		virtual void Explosin();
	
};
