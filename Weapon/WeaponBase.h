
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/AttackInterface/I_Fire.h"
#include "WeaponBase.generated.h"

UCLASS()
class FPS_TESTGAME_API AWeaponBase : public AActor , public II_Fire
{
	GENERATED_BODY()
	
public:	
	AWeaponBase();

	UFUNCTION(BlueprintCallable)
		float GetAttackHP()const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TrenchName")
		FName TrenchName;

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeponeBasicValue")
		float AttackHP_Value;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeponeBasicValue")
		float AttackTimeInterval;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual bool Fire_Int_Implementation(bool isFire,float Time);

	UFUNCTION(BlueprintCallable)
	virtual bool OnAttack();
	UFUNCTION(BlueprintCallable)
	virtual bool OffAttack();

};
