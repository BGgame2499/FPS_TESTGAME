
#include "WeaponBase.h"
#include "Components/StaticMeshComponent.h"
#include "Engine.h"

AWeaponBase::AWeaponBase()
{
	PrimaryActorTick.bCanEverTick = true;
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
	
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, "WeponeFire_Int",true);
	return false;
}

