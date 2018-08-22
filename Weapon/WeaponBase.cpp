
#include "WeaponBase.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "Interface/AddWeaponInterface/I_AddWeapon.h"
#include "Engine.h"

AWeaponBase::AWeaponBase()
{
	AttackHP_Value = 25.f;
	AttackTimeInterval = 1.5f;
	PrimaryActorTick.bCanEverTick = true;
	TrenchName = "NULL";
	MaxReserveBullet = 120;
	MaxCurrentBullet = 30;
	ReserveBullet = MaxReserveBullet;
	CurrentBullet = MaxCurrentBullet;
	WeaponTime = 0.f;
	WeaponHitTime = 1.5f;
	isHit = false;
	isAttackFire = false;
	ThisWeaponSpeciesEnum = WeaponSpeciesEnum::SK_KA74U;	//当前武器的种类


	//WeaponRootScene = CreateDefaultSubobject<USceneComponent>(TEXT("WeaponRootScene"));

	//WeaponStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponStaticMesh"));
	//WeaponStaticMesh->SetSimulatePhysics(true);
	//WeaponStaticMesh->SetupAttachment(GetRootComponent());

	WeaponHitSphere = CreateDefaultSubobject<USphereComponent>(TEXT("WeaponHitSphere"));
	WeaponHitSphere->SetSphereRadius(55.0f);
	WeaponHitSphere->SetupAttachment(GetRootComponent());
	WeaponHitSphere->OnComponentBeginOverlap.AddDynamic(this, &AWeaponBase::BeginHit);


	//WeaponBox = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponBox"));
	//WeaponBox->SetSimulatePhysics(true);

}

float AWeaponBase::GetAttackHP() const
{
	return AttackHP_Value;
}

void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();

	this->SetActorRotation(FRotator(-90, 0, 0));


	//设置碰撞参数，Tag为一个字符串用于以后识别，true是TraceParams.bTraceComplex，this是InIgnoreActor    
	FCollisionQueryParams TraceParams(FName(TEXT("TraceUsableActor")), true, this);    
	TraceParams.bTraceAsyncScene = true;    
	TraceParams.bReturnPhysicalMaterial = false;    //使用复杂Collision判定，逐面判定，更加准确        
	TraceParams.bTraceComplex = true;    /* FHitResults负责接受结果 */ 
	TraceParams.AddIgnoredActor(this);

	FHitResult Hit(ForceInit);
	if (GetWorld()->LineTraceSingleByChannel(Hit,GetActorLocation(), FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z - 1000), ECollisionChannel::ECC_Camera, TraceParams))
	{
		//SetActorLocation(FVector(Hit.Location.X, Hit.Location.Y, Hit.Location.Z+2));
	}
	
}

void AWeaponBase::SetCurrentMeshCollision(bool bCollision)
{
}

void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	WeaponTime += DeltaTime;
	if (WeaponTime >= WeaponHitTime && WeaponTime <= WeaponHitTime + DeltaTime)
	{
		isHit = true;
	}

}

bool AWeaponBase::Fire_Int_Implementation(bool isFire, float Time)
{

	if (isFire)
	{
		if (IsCurrentBullet())
		{
			isAttackFire = true;
			
		}
	}
	else 
	{
		isAttackFire = false;
	}
	return false;
}

void AWeaponBase::BeginHit(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (isHit)
	{
		II_AddWeapon * AddWeaponInter = Cast<II_AddWeapon>(OtherActor);
		if (AddWeaponInter)
		{
			if (AddWeaponInter->Execute_AddWeapon_Int(OtherActor, this))
			{
				SetCurrentMeshCollision(false);
				isHit = false;
			}
			else
			{
				SetCurrentMeshCollision(true);

			}
		}
	}

}

bool AWeaponBase::SetWeaponBullet(int32 Currentbullet, int32 Reservebullet)
{
	if (Currentbullet >= 0 && Currentbullet <= MaxCurrentBullet && Reservebullet >= 0 && Reservebullet <= MaxReserveBullet)
	{
		CurrentBullet = Currentbullet;
		ReserveBullet = Reservebullet;
		return true;
	}

	return false;
}

void AWeaponBase::AddWeaponBullet(int32 Reservebullet)
{
	if (Reservebullet > 0)
	{
		int32 addNum = Reservebullet + ReserveBullet;
		if (addNum > MaxReserveBullet)
		{
			ReserveBullet = MaxReserveBullet;
		}
	}
}
bool AWeaponBase::Reload()
{
	return false;
}

void AWeaponBase::OnAttack()
{
}

void AWeaponBase::OffAttack()
{
}


AWeaponBase * AWeaponBase::ClonWeapon(FTransform ClonWeaponTransform)
{
	AWeaponBase * newWeapon = GetWorld()->SpawnActor<AWeaponBase>(GetClass(), ClonWeaponTransform);
	if (newWeapon)
	{
		newWeapon->SetWeaponBullet(CurrentBullet, ReserveBullet);
		return newWeapon;
	}
	return nullptr;
}

bool AWeaponBase::IsReserveBullet()
{
	if (ReserveBullet > 0)
	{
		return true;
	}
	return false;
}

bool AWeaponBase::IsCurrentBullet()
{
	if (CurrentBullet > 0)
	{
		return true;
	}
	return false;
}
