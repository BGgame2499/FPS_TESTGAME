
#include "WeaponGun.h"
#include "Components/SkeletalMeshComponent.h"
#include "PlayerCharacter/PlayerCharacterBase.h"
#include "Kismet/GameplayStatics.h"
#include "Engine.h"

static int32 DebugWeaponDrawing = 0;
FAutoConsoleVariableRef CVARDebugWeaponDrawing(TEXT("KZQ.DebugWeaponLine"), DebugWeaponDrawing, TEXT("WeaponLine"), ECVF_Cheat);

static int32 WeaponFireShake = 1;
FAutoConsoleVariableRef CVARWeaponFireShake(TEXT("KZQ.WeaponFireShake"), DebugWeaponDrawing, TEXT("WeaponLine"), ECVF_Cheat);


AWeaponGun::AWeaponGun()
{
	CurrentMuzzleTransform = FTransform();
	CurrentMuzzleName = "Muzzle";
	ShellEjectionName = "ShellEjectionSocket";
	WeaponSkletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponSkletalMesh"));
	WeaponSkletalMesh->SetupAttachment(WeaponHitSphere);
	WeaponSkletalMesh->SetSimulatePhysics(false);

	GunMuzzleOffset = FVector(0.0f, 0.0f, 0.0f);


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

	APlayerCharacterBase * MyOwner = Cast<APlayerCharacterBase>(GetOwner());

	if (MyOwner, IsCurrentBullet())
	{
		APlayerCharacterBase * pawn = Cast<APlayerCharacterBase>(GetWorld()->GetFirstPlayerController()->GetPawn());

		//设置碰撞参数，Tag为一个字符串用于以后识别，true是TraceParams.bTraceComplex，this是InIgnoreActor    

		BulletSpawnRotation = pawn->CameraComp->GetComponentRotation();
		// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
		BulletSpawnLocation = (CurrentMuzzleTransform.GetLocation() + BulletSpawnRotation.RotateVector(GunMuzzleOffset));

		//Set Spawn Collision Handling Override
		/*FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;*/

		/////////////////////////////////////////////////////////////   子弹射线检测
		FVector EyeLocation;
		FRotator EyeRotation;

		if (MyOwner->CameraComp)
		{
			EyeLocation = MyOwner->CameraComp->GetComponentLocation();
		    EyeRotation = MyOwner->CameraComp->GetComponentRotation();
		}
		else
		{
			MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);
		}

		FVector ShotDirection = EyeRotation.Vector();
		FVector TraceEnd = EyeLocation + (ShotDirection * 10000);

		FCollisionQueryParams TraceParams;
		//TraceParams.bTraceAsyncScene = true;
	    //TraceParams.bReturnPhysicalMaterial = false;    //使用复杂Collision判定，逐面判定，更加准确        
		TraceParams.bTraceComplex = true;    /* FHitResults负责接受结果 */
		TraceParams.AddIgnoredActor(this);
		TraceParams.AddIgnoredActor(MyOwner);
		
		FHitResult Hit;
		if (GetWorld()->LineTraceSingleByChannel(Hit, EyeLocation, TraceEnd, ECollisionChannel::ECC_Visibility, TraceParams))
		{
			AActor * HitActor = Hit.GetActor();

			UGameplayStatics::ApplyPointDamage(HitActor, 10.0f, ShotDirection, Hit, MyOwner->GetInstigatorController(), this, DamageType);
			
			
		}
		if (DebugWeaponDrawing > 0)
		{
			DrawDebugLine(GetWorld(), EyeLocation, TraceEnd, FColor::Blue, false, 2.0f);
		}
		
		/////////////////////////////////////////////////////////////////

		GetWorld()->SpawnActor<AActor>(BulletActorClass, BulletSpawnLocation, EyeRotation);

		PlayWeaponParticle();

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

void AWeaponGun::PlayWeaponParticle()
{
	if (FireParticle) { UGameplayStatics::SpawnEmitterAttached(FireParticle, WeaponSkletalMesh, CurrentMuzzleName); }
	if (FireMuzzleSmokeParticle) { UGameplayStatics::SpawnEmitterAttached(FireMuzzleSmokeParticle, WeaponSkletalMesh, CurrentMuzzleName); }
	if (FireShellEjectionParticle) { UGameplayStatics::SpawnEmitterAttached(FireShellEjectionParticle, WeaponSkletalMesh, ShellEjectionName); }
	if (FireSound) { UGameplayStatics::SpawnSoundAttached(FireSound, this->GetRootComponent()); }

	APawn * MyOwner = Cast<APawn>(GetOwner());
	if (MyOwner)
	{
		APlayerController * PC = Cast<APlayerController>(MyOwner->GetController());
		if (PC && WeaponFireShake > 0)
		{
			PC->ClientPlayCameraShake(FireCameraShake);
		}
	}
}
