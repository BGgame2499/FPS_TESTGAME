
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacterBase.generated.h"

UCLASS()
class FPS_TESTGAME_API APlayerCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	APlayerCharacterBase();


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent * PlayerMeshStatic;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USpringArmComponent * CameraBoomComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UCameraComponent * CameraComp;
	
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "PlayerState")
	bool IsDie;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerState")
	int32 MaxHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerState")
	int32 MinHP; 



	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerWeapone")
		TSubclassOf<class AWeaponGun > DefaultWeaponClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerWeapone")
		class AWeaponGun * Gun_A;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerWeapone")
		class AWeaponGun * Gun_B;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerWeapone")
		class AWeaponFire * FireWeapon_A;
		
private:
	int32 HP;
protected:
	virtual void BeginPlay() override;
public:
	UFUNCTION(BlueprintCallable)
	int32 GetHP();

	UFUNCTION(BlueprintCallable)
	int32 AddHP(int32 hp);
public:	
	virtual void Tick(float DeltaTime) override;

	void ExamineHP();
	
};
