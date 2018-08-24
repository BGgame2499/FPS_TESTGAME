
#include "WeaponBomb.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "Engine.h"


AWeaponBomb::AWeaponBomb()
{
	RadialForceRadius = 300.0f;


	RadialForceComp = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialForceComp"));
	RadialForceComp->SetupAttachment(WeaponHitSphere);
	RadialForceComp->Radius = RadialForceRadius;
	RadialForceComp->bImpulseVelChange = true;
	RadialForceComp->bAutoActivate = false;
	RadialForceComp->bIgnoreOwningActor = true;
}

void AWeaponBomb::BeginPlay()
{
	Super::BeginPlay();
	RadialForceComp->FireImpulse();
}

void AWeaponBomb::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
