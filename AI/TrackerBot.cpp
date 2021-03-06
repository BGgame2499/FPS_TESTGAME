// This code was written by 康子秋

#include "TrackerBot.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem/Public/NavigationPath.h"
#include "NavigationSystem.h"
#include "DrawDebugHelpers.h"
#include "Components/HealthComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Sound/SoundCue.h"
#include "PlayerCharacter/PlayerCharacterBase.h"
#include "Components/SphereComponent.h"
ATrackerBot::ATrackerBot()
{
	MovementForce = 1000.0f;	//默认物理移动力度
	bUseVelocityChange = true;	//是否开启追踪
	bStartedSelfDestruction = false;	//是否开启计时伤害
	bExploded = false;	//是否已经爆炸
	RequiredDistanceToTarget = 100.0f;	//查找下一个点位所需半径
	ExplosionDamage = 50.0f;	//默认爆炸造成的伤害
	ExplosionRadius = 300.0f;	//默认爆炸范围
	DamageSelfVolume = 10.0f;	//默认自我伤害值

	PrimaryActorTick.bCanEverTick = true;
	BotMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	BotMeshComp->SetCanEverAffectNavigation(false);
	BotMeshComp->SetSimulatePhysics(true);
	RootComponent = BotMeshComp;

	HealthComp = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComp"));
	HealthComp->OnHealthChanged.AddDynamic(this, &ATrackerBot::HanldTakeDamage);
	//FloatMoveComp = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("FloatMoveComp"));

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SphereComp->SetSphereRadius(ExplosionRadius);
	SphereComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ATrackerBot::SphereBeginOverlap);
	SphereComp->SetupAttachment(RootComponent);


}

void ATrackerBot::BeginPlay()
{
	Super::BeginPlay();
	
	if (Role == ROLE_Authority)
	{
		NextPathPoint = FindNextPathPoint();	//找到起始点位
	}

}

FVector ATrackerBot::FindNextPathPoint()
{
	//得到玩家指针并获取路径
	ACharacter * PlayerPawn = UGameplayStatics::GetPlayerCharacter(this, 0);
	UNavigationPath * NavPath = UNavigationSystemV1::FindPathToActorSynchronously(this, GetActorLocation(), PlayerPawn);
	
	if (NavPath && NavPath->PathPoints.Num() > 1)
	{
		return NavPath->PathPoints[1];
	}

	return GetActorLocation();
}

 void ATrackerBot::HanldTakeDamage(UHealthComponent * HealthComponent, float Health, float HealthDelta, const UDamageType * DamageType, AController * InstigatedBy, AActor * DamageCauser)
 {
	if (MatInst == nullptr)
	{
		 MatInst = BotMeshComp->CreateAndSetMaterialInstanceDynamicFromMaterial(0, BotMeshComp->GetMaterial(0));
	}
	if (MatInst)
	{
		MatInst->SetScalarParameterValue("LastTimeDamageTaken", GetWorld()->TimeSeconds);
	}
	UE_LOG(LogTemp, Log, TEXT("Health %s of %s"), *FString::SanitizeFloat(Health), *GetName());

	if (Health <= 0.0f)
	{
		SelfDestruct();
	}
 
 }

 void ATrackerBot::SelfDestruct()
 {
	 if (bExploded)
	 {
		 return;
	 }
	 bExploded = true;

	 UGameplayStatics::SpawnSoundAtLocation(GetWorld(), ExplosionSound, GetActorLocation());
	 UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, GetActorLocation());

	 BotMeshComp->SetVisibility(false, true);
	 BotMeshComp->SetSimulatePhysics(false);
	 BotMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	 if (Role == ROLE_Authority)
	 {
		 TArray<AActor*> IgnoredActors;
		 IgnoredActors.Add(this);
		 UGameplayStatics::ApplyRadialDamage(this, ExplosionDamage, GetActorLocation(), ExplosionRadius, nullptr, IgnoredActors, this, GetInstigatorController(), true);

		 //DrawDebugSphere(GetWorld(), GetActorLocation(), ExplosionRadius, 12, FColor::Red, false, 2.0f, 0, 1.0f);
		 SetLifeSpan(1.0f);	//两秒后销毁
	 }
 }

 void ATrackerBot::DamageSelf()
 {
	 UGameplayStatics::ApplyDamage(this, DamageSelfVolume, GetInstigatorController(), this, nullptr);
 }

void ATrackerBot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (Role == ROLE_Authority && !bExploded)
	{
		float DistanceToTarget = (GetActorLocation() - NextPathPoint).Size();

		if (DistanceToTarget <= RequiredDistanceToTarget)
		{
			NextPathPoint = FindNextPathPoint();	//找到下一个点位

			//DrawDebugString(GetWorld(), GetActorLocation(), "CompleteToTarget");
		}
		else
		{
			FVector ForceDirection = NextPathPoint - GetActorLocation();
			ForceDirection.Normalize();
			ForceDirection *= MovementForce;

			BotMeshComp->AddForce(ForceDirection, NAME_None, bUseVelocityChange);
		}
	}
}

void ATrackerBot::SphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (!bStartedSelfDestruction && !bExploded)
	{	
		APlayerCharacterBase * PlayerPawn = Cast<APlayerCharacterBase>(OtherActor);

		if (PlayerPawn)	//检查到碰撞为玩家
		{
			if (Role == ROLE_Authority)
			{
				GetWorldTimerManager().SetTimer(TimeHandle_SelfDamage, this, &ATrackerBot::DamageSelf, 0.5f, true, 0.0f);	//倒计时自动销毁
			}
			UGameplayStatics::SpawnSoundAttached(SelfDestructSound, RootComponent);
			bStartedSelfDestruction = true;
		}
	}

}

