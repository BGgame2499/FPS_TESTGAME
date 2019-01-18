
#include "HealthComponent.h"
#include "GameMode/FPS_TESTGAMEGameModeBase.h"
#include "Net/UnrealNetwork.h"	


UHealthComponent::UHealthComponent()
{
	
	PrimaryComponentTick.bCanEverTick = false;

	DefaultHealth = 100.0f;

	SetIsReplicated(true);
	bIsDead = false;
}


void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	
	if (GetOwnerRole() == ROLE_Authority)	//如果当前位置是服务器  Only hook if we serve
	{
		AActor * MyOwner = GetOwner();
		if (MyOwner)
		{
			MyOwner->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::HandleTakeAnyDamage);	//Bind the Actor for TakeAnyDamage event
		}
	}
	
	Health = DefaultHealth;
}

void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UHealthComponent::HandleTakeAnyDamage(AActor * DamagedActor, float Damage, const UDamageType * DamageType, AController * InstigatedBy, AActor * DamageCauser)
{
	if (Damage <= 0.0f || bIsDead)
	{
		return;
	}

	Health = FMath::Clamp(Health - Damage, 0.0f, DefaultHealth);

	bIsDead = Health <= 0.0f;

	OnHealthChanged.Broadcast(this, Health, Damage, DamageType, InstigatedBy, DamageCauser);	//广播事件

	UE_LOG(LogTemp, Log, TEXT("Health Changed : %s"), *FString::SanitizeFloat(Health));

	if (bIsDead)	//如果当前角色已死亡则向GameMode广播消息
	{
		AFPS_TESTGAMEGameModeBase * GM = Cast<AFPS_TESTGAMEGameModeBase>(GetWorld()->GetAuthGameMode());
		if (GM)
		{
			GM->OnActorKilled.Broadcast(GetOwner(), DamageCauser, InstigatedBy);
		}
	}


}

void UHealthComponent::OnRep_Health(float OldHealth)
{
	float Damage = Health - OldHealth;

	OnHealthChanged.Broadcast(this, Health, Damage, nullptr, nullptr, nullptr);
}

float UHealthComponent::GetHealth() const
{
	return Health;
}

bool UHealthComponent::Heal(float HealthAmount)
{
	if (HealthAmount <= 0 || Health <= 0 || Health >= DefaultHealth)
	{
		return false;
	}

	Health = FMath::Clamp(Health + HealthAmount, 0.0f, DefaultHealth);

	OnHealthChanged.Broadcast(this, Health, -HealthAmount, nullptr, nullptr, nullptr);

	return true;
}

void UHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const	//成员复制
{

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UHealthComponent, Health); //COND_SkipOwner防止客户端将参数复制到服务器后服务器再次通知此客户端（此参数回调的特效函数已经执行过了 避免服务器通知二次执行）
}
