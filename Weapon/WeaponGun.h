// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/WeaponBase.h"
#include "WeaponGun.generated.h"

class UDamageType;
class UCameraShake;
class UMaterialInterface;

USTRUCT()
struct FHitScanTrace
{
	GENERATED_BODY()

public:
	UPROPERTY()
		TEnumAsByte< EPhysicalSurface> SurfaceType;

	UPROPERTY()
	FVector_NetQuantize TraceTo;

};

UCLASS()
class FPS_TESTGAME_API AWeaponGun : public AWeaponBase
{
	GENERATED_BODY()
public:
	AWeaponGun();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USkeletalMeshComponent * WeaponSkletalMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "WeponeBasicValue")
	FName CurrentMuzzleName; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeponeBasicValue")
	FName ShellEjectionName; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeponeBasicValue")
	FTransform CurrentMuzzleTransform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeponeBasicValue")
		TSubclassOf<AActor> BulletActorClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeponeBasicValue")
		TSubclassOf< UDamageType > DamageType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeponeBasicValue")	//武器攻击物理力度
		float AttackLinearVelocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeponeBasicValue")
		FVector2D RandomRecoilPith;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeponeBasicValue")
		FVector2D RandomRecoilYaw;
	////////////////////////////////////////////////////武器特效
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particle")
		UParticleSystem * FireParticle;		//开火特效
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particle")
		UParticleSystem * FireMuzzleSmokeParticle;		//开火烟特效
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particle")
		UParticleSystem * FireShellEjectionParticle;		//开火弹壳特效
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particle")
		UParticleSystem * DefaultImpactEffect;				//默认击中特效
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particle")
		TArray<UParticleSystem *> ImpactParticleArray;		//击中特效数组
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponSound")
		USoundCue * FireSound;		//开火音效
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decal")
		TArray<UMaterialInterface *> ImpactDecalArray;	//击中单孔贴花数组
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decal")
		UMaterialInterface * DefaultImpactDecal;		//击中单孔贴花
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "WeaponFireShake")
		TSubclassOf<UCameraShake> FireCameraShake;

	////////////////////////////////////////////////////武器参数
	float  LastFireTime;   //记录之前开火的时间

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeponeBasicValue")
	float  RateOfFire;   //每分钟的开火速率

	float  TimeBetweenShots;  //获取到开火时间间隔

	FTimerHandle  TimerHandle_WeaponFireTimeHand;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ContinuousFire")
	bool  bContinuousFire;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BulletFireValue")
	FVector GunMuzzleOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BulletFireValue")
	FRotator BulletSpawnRotation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BulletFireValue")
	FVector BulletSpawnLocation;


public:
	UFUNCTION(BlueprintCallable)
		UParticleSystem * GetImpactParticle(EPhysicalSurface  SurfaceType);

	void PlayImpactEffects(EPhysicalSurface  SurfaceType,FVector ImpactPoint);

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerOnAttack();

	UPROPERTY(ReplicatedUsing = OnRep_HitScanTrace)
		FHitScanTrace HitScanTrace;

	UFUNCTION()
		void OnRep_HitScanTrace();


private:
protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void OnAttack()override;
	virtual void OffAttack()override;

		virtual void PlayWeaponParticle() override;		//武器特效集中在此

		virtual bool Fire_Int_Implementation(bool isFire, float Time) override;

		virtual void SetCurrentMeshCollision(bool bCollision) override;
};
