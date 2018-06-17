
#include "WeaponBase.h"
#include "Components/StaticMeshComponent.h"
#include "Engine.h"

AWeaponBase::AWeaponBase()
{
	AttackHP_Value = 25.f;
	AttackTimeInterval = 1.5f;
	PrimaryActorTick.bCanEverTick = true;
	TrenchName = "NULL";
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

bool AWeaponBase::OnAttack()
{
	return false;
}

bool AWeaponBase::OffAttack()
{
	return false;
}

