
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
	USkeletalMeshComponent * PlayerMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent * PlayerMeshStatic;

public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "PlayerState")
	bool IsDie;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerState")
	int32 MaxHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerState")
	int32 MinHP;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerWeapone")
		class WeaponGunBase * Gun_A;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerWeapone")
		class WeaponGunBase * Gun_B;

private:
	int32 HP;
protected:
	virtual void BeginPlay() override;
public:
	int32 GetHP();

	int32 AddHP(int32 hp);
public:	
	virtual void Tick(float DeltaTime) override;

	void ExamineHP();
	
};
