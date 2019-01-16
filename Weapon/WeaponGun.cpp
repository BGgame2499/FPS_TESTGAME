
#include "WeaponGun.h"
#include "Components/SkeletalMeshComponent.h"
#include "PlayerCharacter/PlayerCharacterBase.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "FPS_TESTGAME.h"
#include "TimerManager.h"
#include "Net/UnrealNetwork.h"
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
	//WeaponSkletalMesh->SetSimulatePhysics(true);
	WeaponHitSphere->SetupAttachment(WeaponSkletalMesh);
	SetRootComponent(WeaponSkletalMesh);
	WeaponSkletalMesh->SetCanEverAffectNavigation(false);
	//WeaponSkletalMesh->MoveComponent(WeaponSkletalMesh->GetComponentLocation(), WeaponSkletalMesh->GetComponentRotation(), false, nullptr, MOVECOMP_IgnoreBases, ETeleportType::TeleportPhysics);

	GunMuzzleOffset = FVector(0.0f, 0.0f, 0.0f);
	AttackLinearVelocity = 1000.0f;

	RandomRecoilPith = FVector2D(0.9,1.1);
	RandomRecoilYaw = FVector2D(-1.0,1.0);
	RateOfFire = 400.0f;   //默认每分钟的开火速率

	CurrentMuzzleTransform = WeaponSkletalMesh->GetSocketTransform(CurrentMuzzleName);

	NetUpdateFrequency = 66.0f;		//设置武器网络更新频率
	MinNetUpdateFrequency = 33.0f;

	bContinuousFire = true;
}

void AWeaponGun::BeginPlay()
{
	Super::BeginPlay();

	TimeBetweenShots = 60 / RateOfFire;  //获取到开火时间间隔
}

void AWeaponGun::SetCurrentMeshCollision(bool bCollision)	//武器模型的碰撞设置
{
	if (bCollision)
	{
		WeaponSkletalMesh->SetSimulatePhysics(true);
		WeaponSkletalMesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	}
	else
	{

		WeaponSkletalMesh->SetSimulatePhysics(false);
		WeaponSkletalMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AWeaponGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeaponGun::OnAttack()	//开火
{
	if (Role < ROLE_Authority)
	{
		ServerOnAttack();
	}


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
		
		FVector TraceEndPoint = TraceEnd;

		EPhysicalSurface SurfaceType = SurfaceType_Default;

		FHitResult Hit;
		if (GetWorld()->LineTraceSingleByChannel(Hit, EyeLocation, TraceEnd, COLLISION_WEAPON, TraceParams))
		{
			AActor * HitActor = Hit.GetActor();

			/////////////////////////////////////////////////////////////   子弹碰撞物理材质响应

			SurfaceType = UPhysicalMaterial::DetermineSurfaceType(Hit.PhysMaterial.Get());

			float ActualDemage = AttackHP_Value;
			if (SurfaceType == SurfaceType2)
			{
				ActualDemage *= 4.0;
			}


			if (Hit.GetComponent()->IsSimulatingPhysics()) { Hit.GetComponent()->AddImpulse(ShotDirection * AttackLinearVelocity, NAME_None, true); }//施加力
			UGameplayStatics::ApplyPointDamage(HitActor, ActualDemage, ShotDirection, Hit, MyOwner->GetInstigatorController(), this, DamageType);	//施加伤害


			PlayImpactEffects(SurfaceType, Hit.ImpactPoint);	//播放击中特效

			/////////////////////////////////////////////////////////////////
			TraceEndPoint = Hit.ImpactPoint;
			
		}
		

		//GetWorld()->SpawnActor<AActor>(BulletActorClass, BulletSpawnLocation, EyeRotation);

		PlayWeaponParticle();	//播放各个武器特效

		if (Role == ROLE_Authority)		//记录当前武器击中位置和材质类型
		{
			HitScanTrace.TraceTo = TraceEndPoint;
			HitScanTrace.SurfaceType = SurfaceType;
		}

		LastFireTime = GetWorld()->TimeSeconds;	 //记录最后一次开火时间

		MyOwner->RecoilRifleFire(RandomRecoilPith, RandomRecoilYaw);	//设置后坐力

		if (DebugWeaponDrawing > 0)
		{
			DrawDebugLine(GetWorld(), EyeLocation, TraceEnd, FColor::Blue, false, 2.0f);
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "OnAttackFire", true);
		}
	}
	else
	{

	}
}

void AWeaponGun::ServerOnAttack_Implementation()
{
	OnAttack();
}
bool AWeaponGun::ServerOnAttack_Validate()
{
	return true;
}

void AWeaponGun::OffAttack()	//停火
{
	GetWorldTimerManager().ClearTimer(TimerHandle_WeaponFireTimeHand);


	if (DebugWeaponDrawing > 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "OffAttackFire", true);
	}
}

void AWeaponGun::PlayWeaponParticle()
{
	if (FireParticle) { UGameplayStatics::SpawnEmitterAttached(FireParticle, WeaponSkletalMesh, CurrentMuzzleName); }
	if (FireMuzzleSmokeParticle) { UGameplayStatics::SpawnEmitterAttached(FireMuzzleSmokeParticle, WeaponSkletalMesh, CurrentMuzzleName); }
	if (FireShellEjectionParticle) {UGameplayStatics::SpawnEmitterAttached(FireShellEjectionParticle, WeaponSkletalMesh, ShellEjectionName); }
	if (FireSound) { UGameplayStatics::SpawnSoundAttached(FireSound, this->GetRootComponent()); }
	APawn * MyOwner = Cast<APawn>(GetOwner());		//相机抖动
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

			if (bContinuousFire)	//如果是自动武器则开启连续射击
			{
				GetWorldTimerManager().SetTimer(TimerHandle_WeaponFireTimeHand, this, &AWeaponGun::OnAttack, TimeBetweenShots, true, FirstDelay);
			}
			else
			{
				GetWorldTimerManager().SetTimer(TimerHandle_WeaponFireTimeHand, this, &AWeaponGun::OnAttack, TimeBetweenShots, false, FirstDelay);
				isAttackFire = false;
			}

		}
	}
	else
	{
		isAttackFire = false;
		OffAttack();
	}

	return false;


}
UParticleSystem * AWeaponGun::GetImpactParticle(EPhysicalSurface  SurfaceType)	//解析表面材质
{
	UParticleSystem * ImpactParticle = nullptr;
	int32 Index = 0;
	switch (SurfaceType)
	{
	case SurfaceType_Default:
		break;
	case SurfaceType1:	//击中肉体
		Index = 1;
		break;
	case SurfaceType2:	//击中头部
		Index = 2;
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
		break;
	}

	ImpactParticle = ImpactParticleArray.IsValidIndex(Index) ? ImpactParticleArray[Index] : DefaultImpactEffect;	//查找并得到特效
	DefaultImpactDecal = ImpactDecalArray.IsValidIndex(Index) ? ImpactDecalArray[Index] : ImpactDecalArray[0];		//得到弹孔印花

	return ImpactParticle;
}

void AWeaponGun::PlayImpactEffects(EPhysicalSurface SurfaceType, FVector ImpactPoint)
{
	UParticleSystem * ImpactParticle = GetImpactParticle(SurfaceType);

	if (ImpactParticle)
	{
		FVector MuzzleLocation = WeaponSkletalMesh->GetSocketLocation(CurrentMuzzleName);

		FVector ShotDirection = ImpactPoint - MuzzleLocation;
		ShotDirection.Normalize();
		//FRotator ParticleRotator = FRotator(ShotDirection.Rotation().Yaw, ShotDirection.Rotation().Pitch, ShotDirection.Rotation().Roll);
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticle,ImpactPoint, ShotDirection.Rotation());		//创建击中特效

		UMaterialInstanceDynamic* DynMaterial = UMaterialInstanceDynamic::Create(DefaultImpactDecal, this);	//得到印花材质并创建动态材质
		if (DynMaterial)
		{
			int32 RandomNum = FMath::RandRange(1, 4);
			DynMaterial->SetScalarParameterValue("Frame", RandomNum);	//设置材质变量
			FVector DecalSize = FVector(5, 5, 5) * FMath::FRandRange(0.75, 1.9);
			UGameplayStatics::SpawnDecalAtLocation(GetWorld(), DynMaterial, DecalSize, ImpactPoint, ShotDirection.Rotation());	//创建印花
		}
	}
}

void AWeaponGun::OnRep_HitScanTrace()
{

	PlayWeaponParticle();	//播放各个武器特效


	PlayImpactEffects(HitScanTrace.SurfaceType, HitScanTrace.TraceTo);
}

void AWeaponGun::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const	//成员复制
{

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(AWeaponGun, HitScanTrace,COND_SkipOwner); //COND_SkipOwner防止客户端将参数复制到服务器后服务器再次通知此客户端（此参数回调的特效函数已经执行过了 避免服务器通知二次执行）
}
