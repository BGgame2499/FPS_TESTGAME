
#include "WeaponGun.h"
#include "Components/SkeletalMeshComponent.h"

AWeaponGun::AWeaponGun()
{
	WeaponSkletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponSkletalMesh"));
}

void AWeaponGun::BeginPlay()
{
	Super::BeginPlay();

}

void AWeaponGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

