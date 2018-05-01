
#include "PlayerCharacterBase.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"

APlayerCharacterBase::APlayerCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;
	IsDie = false;
	MaxHP = 100;
	MinHP = 0;
	HP = MaxHP;

	PlayerMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("PlayerMesh"));
	PlayerMesh->SetupAttachment(GetRootComponent());

	PlayerMeshStatic = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlayerMeshStatic"));
	PlayerMeshStatic->SetupAttachment(PlayerMesh);
}

void APlayerCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}
void APlayerCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ExamineHP();
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

