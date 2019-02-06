
#include "WeaponBomb.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Engine.h"


AWeaponBomb::AWeaponBomb()
{
	ExplosinDelayTime = 3.0f;

	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComp"));
	WeaponHitSphere->SetupAttachment(MeshComp);

	RadialForceComp = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialForceComp"));
	RadialForceComp->SetupAttachment(MeshComp);
	RadialForceComp->bImpulseVelChange = true;
	RadialForceComp->bAutoActivate = false;
	RadialForceComp->bIgnoreOwningActor = true;
	RadialForceComp->DestructibleDamage = 30.0f;
}

void AWeaponBomb::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(ExplosionTimeHandle,this,&AWeaponBomb::Explosin,ExplosinDelayTime,false);

}

void AWeaponBomb::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeaponBomb::Explosin()	//爆炸函数
{
	if (ExplosionEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, GetActorLocation());
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), ExplosionSound, GetActorLocation());
	}
	TArray<AActor*> IgnoredActors;
	IgnoredActors.Add(this);
	UGameplayStatics::ApplyRadialDamage(this, AttackHP_Value, GetActorLocation(), RadialForceComp->Radius, nullptr, IgnoredActors, this, GetInstigatorController(), true);

	RadialForceComp->FireImpulse();	//施加力量
	Destroy();
}
