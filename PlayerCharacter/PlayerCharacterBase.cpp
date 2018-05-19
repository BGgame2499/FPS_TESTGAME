
#include "PlayerCharacterBase.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Weapon/WeaponFire.h"
#include "Weapon/WeaponGun.h"
#include "Components/InputComponent.h"
#include "Engine.h"
APlayerCharacterBase::APlayerCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;
	IsDie = false;
	MaxHP = 100;
	MinHP = 0;
	HP = MaxHP;

	PlayerMeshStatic = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlayerMeshStatic"));
	PlayerMeshStatic->SetupAttachment(GetRootComponent());

	CameraBoomComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoomComp"));
	CameraBoomComp->SetupAttachment(GetRootComponent());
	CameraBoomComp->SocketOffset = FVector(0, 35, 0);
	CameraBoomComp->TargetOffset = FVector(0, 0, 55);
	CameraBoomComp->bUsePawnControlRotation = true;	//ÔÊÐí¸úËæ½ÇÉ«Ðý×ª

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(CameraBoomComp);

}

void APlayerCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	Gun_A = GetWorld()->SpawnActor<AWeaponGun>(DefaultWeaponClass, FVector(0, 0, 0), FRotator(0, 0, 0));
	
	Gun_A->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), "Finger_04_R");
}
void APlayerCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ExamineHP();
}
void APlayerCharacterBase::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this,&APlayerCharacterBase::Jump);

}

int32 APlayerCharacterBase::GetHP()
{
	return HP;
}

int32 APlayerCharacterBase::AddHP(int32 hp)
{
	if (!IsDie)
	{
		if (HP + hp > MaxHP)
		{
			HP = MaxHP;
		}
		else
		{
			HP += hp;
		}
	}
	return HP;
}

void APlayerCharacterBase::ExamineHP()
{
	if (GetHP() <= MinHP)
	{
		HP = MinHP;
		IsDie = true;
	}
}

