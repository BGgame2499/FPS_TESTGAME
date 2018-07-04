
#include "WeaponBase.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
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
	WeaponTime = 0.f;
	isHit = false;


	WeaponHitSphere = CreateDefaultSubobject<USphereComponent>(TEXT("WeaponHitSphere"));
	WeaponHitSphere->SetSphereRadius(55.0f);
	WeaponHitSphere->SetupAttachment(this->GetRootComponent());
	WeaponHitSphere->OnComponentBeginOverlap.AddDynamic(this, &AWeaponBase::BeginHit);
}

float AWeaponBase::GetAttackHP() const
{
	return AttackHP_Value;
}

void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	WeaponTime += DeltaTime;
	if (WeaponTime >= 1.0f&&WeaponTime <= 1.5f)
	{
		isHit = true;
	}
}

bool AWeaponBase::Fire_Int_Implementation(bool isFire, float Time)
{

	if (isFire)
	{
		OnAttack();
	}
	else 
	{
		OffAttack();
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
				isHit = false;
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

bool AWeaponBase::OnAttack()
{
	return false;
}

bool AWeaponBase::OffAttack()
{
	return false;
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
