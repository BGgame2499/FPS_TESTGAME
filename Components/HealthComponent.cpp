
#include "HealthComponent.h"


UHealthComponent::UHealthComponent()
{
	
	PrimaryComponentTick.bCanEverTick = false;

	DefaultHealth = 100.0f;
}


void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	
	AActor * MyOwner = GetOwner();
	if (MyOwner)
	{
		MyOwner->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::HandleTakeAnyDamage);
	}
	Health = DefaultHealth;
}

void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UHealthComponent::HandleTakeAnyDamage(AActor * DamagedActor, float Damage, const UDamageType * DamageType, AController * InstigatedBy, AActor * DamageCauser)
{
	if (Damage <= 0.0f)
	{
		return;
	}

	Health = FMath::Clamp(Health - Damage, 0.0f, DefaultHealth);

	UE_LOG(LogTemp, Log, TEXT("Health Changed : %s"), *FString::SanitizeFloat(Health));
}

