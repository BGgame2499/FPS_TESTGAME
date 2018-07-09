
#include "WeaponGun.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine.h"

AWeaponGun::AWeaponGun()
{
	CurrentMuzzleTransform = FTransform();
	CurrentMuzzleName = "Muzzle";
	ShellEjectionName = "ShellEjectionSocket";
	WeaponSkletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponSkletalMesh"));
	WeaponSkletalMesh->SetupAttachment(WeaponHitSphere);
	WeaponSkletalMesh->SetSimulatePhysics(false);

	CurrentMuzzleTransform = WeaponSkletalMesh->GetSocketTransform(CurrentMuzzleName);
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
	CurrentMuzzleTransform = WeaponSkletalMesh->GetSocketTransform(CurrentMuzzleName);
	if (IsCurrentBullet())
	{
		GetWorld()->SpawnActor<AActor>(BulletActorClass,FVector( CurrentMuzzleTransform.GetLocation()), FRotator( CurrentMuzzleTransform.GetRotation()));
		UGameplayStatics::SpawnEmitterAttached(FireParticle, WeaponSkletalMesh, CurrentMuzzleName);
		UGameplayStatics::SpawnEmitterAttached(FireMuzzleSmokeParticle, WeaponSkletalMesh, CurrentMuzzleName);
		UGameplayStatics::SpawnEmitterAttached(FireShellEjectionParticle, WeaponSkletalMesh, ShellEjectionName);
		UGameplayStatics::SpawnSoundAttached(FireSound, this->GetRootComponent());
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