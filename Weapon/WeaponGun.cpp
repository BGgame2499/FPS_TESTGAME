
#include "WeaponGun.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine.h"

AWeaponGun::AWeaponGun()
{
	CurrentMuzzleLocation = FVector(0,0,0);
	CurrentMuzzleName = "Muzzle";
	WeaponSkletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponSkletalMesh"));
	WeaponSkletalMesh->SetupAttachment(WeaponHitSphere);
	//WeaponSkletalMesh->SetSimulatePhysics(true);
}

void AWeaponGun::BeginPlay()
{
	Super::BeginPlay();
}

void AWeaponGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AWeaponGun::OnAttack()
{
	CurrentMuzzleLocation = WeaponSkletalMesh->GetSocketLocation(CurrentMuzzleName);
	if (CurrentMuzzleLocation != FVector(0,0,0))
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "OnAttackFire", true);
		return true;
	}
	else
	{
		return false;
	}
}
bool AWeaponGun::OffAttack()
{

	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "OffAttackFire", true);
	return false;
}