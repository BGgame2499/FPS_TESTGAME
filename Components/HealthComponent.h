
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

//生命值改变事件
DECLARE_DYNAMIC_MULTICAST_DELEGATE_SixParams(FOnHealthChangedSignhure, UHealthComponent*, HealthComp, float, Health, float, HealthDelta, const class UDamageType*, DamageType, class AController*, InstigatedBy, AActor*, DamageCauser);

UCLASS( ClassGroup=(FPSGAME), meta=(BlueprintSpawnableComponent) )
class FPS_TESTGAME_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UHealthComponent();

	UPROPERTY(BlueprintReadOnly)
	bool bIsDead;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(ReplicatedUsing = OnRep_Health,BlueprintReadOnly, Category = "HealthComponent")
		float Health;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HealthComponent")
		float DefaultHealth;
public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	void HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
	
	UFUNCTION()
	void OnRep_Health(float OldHealth);

	UFUNCTION(BlueprintCallable)
		float GetHealth()const;


public:
	UPROPERTY(BlueprintAssignable, Category = "Events")		//生命改变事件
		FOnHealthChangedSignhure OnHealthChanged;

	UFUNCTION(BlueprintCallable, Category = "HealthComponent")
		bool Heal(float HealthAmount);		//恢复血量

};
