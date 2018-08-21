
#include "WeaponGun.h"
#include "Components/SkeletalMeshComponent.h"
#include "PlayerCharacter/PlayerCharacterBase.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "FPS_TESTGAME.h"
#include "TimerManager.h"
#include "Engine.h"

static int32 DebugWeaponDrawing = 0;
FAutoConsoleVariableRef CVARDebugWeaponDrawing(TEXT("KZQ.DebugWeaponLine"), DebugWeaponDrawing, TEXT("WeaponLine"), ECVF_Cheat);

static int32 WeaponFireShake = 1;
FAutoConsoleVariableRef CVARWeaponFireShake(TEXT("KZQ.WeaponFireShake"), DebugWeaponDrawing, TEXT("WeaponLine"), ECVF_Cheat);


AWeaponGun::AWeaponGun()
{
	CurrentMuzzleName = "Muzzle";
	ShellEjectionName = "ShellEjectionSocket";

	WeaponSkletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponSkletalMesh"));
	WeaponSkletalMesh->SetupAttachment(WeaponHitSphere);

	GunMuzzleOffset = FVector(0.0f, 0.0f, 0.0f);

	RateOfFire = 400.0f;   //默认每分钟的开火速率

	CurrentMuzzleTransform = WeaponSkletalMesh->GetSocketTransform(CurrentMuzzleName);
}

void AWeaponGun::BeginPlay()
{
	Super::BeginPlay();

	TimeBetweenShots = 60 / RateOfFire;  //获取到开火时间间隔
}

void AWeaponGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeaponGun::OnAttack()	//开火
{
	CurrentMuzzleTransform = WeaponSkletalMesh->GetSocketTransform(CurrentMuzzleName);

	APlayerCharacterBase * MyOwner = Cast<APlayerCharacterBase>(GetOwner());

	if (MyOwner, IsCurrentBullet())
	{ 
		/////////////////////////////////////////////////////////////   得到子弹特效发射位置
		BulletSpawnRotation = MyOwner->CameraComp->GetComponentRotation();
		BulletSpawnLocation = (CurrentMuzzleTransform.GetLocation() + BulletSpawnRotation.RotateVector(GunMuzzleOffset));

		/////////////////////////////////////////////////////////////   子弹射线检测
		FVector EyeLocation;
		FRotator EyeRotation;

		if (MyOwner->CameraComp)
		{
			EyeLocation = MyOwner->CameraComp->GetComponentLocation();
		    EyeRotation = MyOwner->CameraComp->GetComponentRotation();
		}
		else
		{
			MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);
		}

		FVector ShotDirection = EyeRotation.Vector();
		FVector TraceEnd = EyeLocation + (ShotDirection * 10000);

		FCollisionQueryParams TraceParams;
		//TraceParams.bTraceAsyncScene = true;
	    //TraceParams.bReturnPhysicalMaterial = false;    //使用复杂Collision判定，逐面判定，更加准确        
		TraceParams.bTraceComplex = true;    /* FHitResults负责接受结果 */
		TraceParams.bReturnPhysicalMaterial = true;
		TraceParams.AddIgnoredActor(this);
		TraceParams.AddIgnoredActor(MyOwner);
		
		FHitResult Hit;
		if (GetWorld()->LineTraceSingleByChannel(Hit, EyeLocation, TraceEnd, COLLISION_WEAPON, TraceParams))
		{
			AActor * HitActor = Hit.GetActor();

			UGameplayStatics::ApplyPointDamage(HitActor, 10.0f, ShotDirection, Hit, MyOwner->GetInstigatorController(), this, DamageType);
			
			
		}
		if (DebugWeaponDrawing > 0)
		{
			DrawDebugLine(GetWorld(), EyeLocation, TraceEnd, FColor::Blue, false, 2.0f);
		}
		
		/////////////////////////////////////////////////////////////   子弹碰撞物理材质响应
		EPhysicalSurface SurfaceType = UPhysicalMaterial::DetermineSurfaceType(Hit.PhysMaterial.Get());
		
		UParticleSystem * ImpactParticle = GetImpactParticle(SurfaceType);

		if (ImpactParticle)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticle, Hit.ImpactPoint, Hit.ImpactNormal.Rotation());
		}

		/////////////////////////////////////////////////////////////////

		//GetWorld()->SpawnActor<AActor>(BulletActorClass, BulletSpawnLocation, EyeRotation);

		PlayWeaponParticle();	//播放各个武器特效


		LastFireTime = GetWorld()->TimeSeconds;	 //记录最后一次开火时间

		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "OnAttackFire", true);
	}
	else
	{

	}
}

void AWeaponGun::OffAttack()	//停火
{
	GetWorldTimerManager().ClearTimer(TimerHandle_WeaponFireTimeHand);
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "OffAttackFire", true);
}

void AWeaponGun::PlayWeaponParticle()
{
	if (FireParticle) { UGameplayStatics::SpawnEmitterAttached(FireParticle, WeaponSkletalMesh, CurrentMuzzleName); }
	if (FireMuzzleSmokeParticle) { UGameplayStatics::SpawnEmitterAttached(FireMuzzleSmokeParticle, WeaponSkletalMesh, CurrentMuzzleName); }
	if (FireShellEjectionParticle) { UGameplayStatics::SpawnEmitterAttached(FireShellEjectionParticle, WeaponSkletalMesh, ShellEjectionName); }
	if (FireSound) { UGameplayStatics::SpawnSoundAttached(FireSound, this->GetRootComponent()); }

	APawn * MyOwner = Cast<APawn>(GetOwner());
	if (MyOwner)
	{
		APlayerController * PC = Cast<APlayerController>(MyOwner->GetController());
		if (PC && WeaponFireShake > 0)
		{
			PC->ClientPlayCameraShake(FireCameraShake);
		}
	}
}
bool AWeaponGun::Fire_Int_Implementation(bool isFire, float Time)
{
	if (isFire)
	{
		if (IsCurrentBullet())
		{
			isAttackFire = true;

			float  FirstDelay = FMath::Max(LastFireTime + TimeBetweenShots - GetWorld()->TimeSeconds, 0.0f);		//计算首次开火的延时（防止玩家连续点击）
			GetWorldTimerManager().SetTimer(TimerHandle_WeaponFireTimeHand,this,&AWeaponGun::OnAttack, TimeBetweenShots, true, FirstDelay);

		}
	}
	else
	{
		isAttackFire = false;
		OffAttack();
	}

	return false;


}
UParticleSystem * AWeaponGun::GetImpactParticle(EPhysicalSurface  SurfaceType)
{
	UParticleSystem * ImpactParticle = nullptr;

	switch (SurfaceType)
	{
	case SurfaceType_Default:

		ImpactParticle = DefaultImpactEffect;
		break;
	case SurfaceType1:	//击中肉体
		ImpactParticle = ImpactParticleArray.IsValidIndex(0) ? ImpactParticleArray[0] : nullptr;
		break;
	case SurfaceType2:	//击中头部
		ImpactParticle = ImpactParticleArray.IsValidIndex(1) ? ImpactParticleArray[1] : nullptr;
		break;
	case SurfaceType3:
		break;
	case SurfaceType4:
		break;
	case SurfaceType5:
		break;
		{
	case SurfaceType6:
		break;
	case SurfaceType7:
		break;
	case SurfaceType8:
		break;
	case SurfaceType9:
		break;
	case SurfaceType10:
		break;
	case SurfaceType11:
		break;
	case SurfaceType12:
		break;
	case SurfaceType13:
		break;
	case SurfaceType14:
		break;
	case SurfaceType15:
		break;
	case SurfaceType16:
		break;
	case SurfaceType17:
		break;
	case SurfaceType18:
		break;
	case SurfaceType19:
		break;
	case SurfaceType20:
		break;
	case SurfaceType21:
		break;
	case SurfaceType22:
		break;
	case SurfaceType23:
		break;
	case SurfaceType24:
		break;
	case SurfaceType25:
		break;
	case SurfaceType26:
		break;
	case SurfaceType27:
		break;
	case SurfaceType28:
		break;
	case SurfaceType29:
		break;
	case SurfaceType30:
		break;
	case SurfaceType31:
		break;
	case SurfaceType32:
		break;
	case SurfaceType33:
		break;
	case SurfaceType34:
		break;
	case SurfaceType35:
		break;
	case SurfaceType36:
		break;
	case SurfaceType37:
		break;
	case SurfaceType38:
		break;
	case SurfaceType39:
		break;
	case SurfaceType40:
		break;
	case SurfaceType41:
		break;
	case SurfaceType42:
		break;
	case SurfaceType43:
		break;
	case SurfaceType44:
		break;
	case SurfaceType45:
		break;
	case SurfaceType46:
		break;
	case SurfaceType47:
		break;
	case SurfaceType48:
		break;
	case SurfaceType49:
		break;
	case SurfaceType50:
		break;
	case SurfaceType51:
		break;
	case SurfaceType52:
		break;
	case SurfaceType53:
		break;
	case SurfaceType54:
		break;
	case SurfaceType55:
		break;
	case SurfaceType56:
		break;
	case SurfaceType57:
		break;
	case SurfaceType58:
		break;
	case SurfaceType59:
		break;
	case SurfaceType60:
		break;
	case SurfaceType61:
		break;
	case SurfaceType62:
		break;
	case SurfaceType_Max:
		break;
		}
	default:
		ImpactParticle = DefaultImpactEffect;
		break;
	}

	return ImpactParticle;
}