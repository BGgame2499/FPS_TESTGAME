
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UStaticMeshComponent * WeaponBaseMesh;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	 virtual bool Fire_Int_Implementation(bool isFire,float Time);

};
