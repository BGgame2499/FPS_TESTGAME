
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/TimelineComponent.h"
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerState")
		int32 SprintSpeedMax;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerState")
		int32 SprintSpeedMin;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerState")
		int32 WalkSpeedMin;

	///////////////////////////////////////////////////////////////////////////////////

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerWeapone")
		TSubclassOf<class AWeaponGun > DefaultWeaponClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerWeapone")
		class AWeaponGun * Gun_A;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerWeapone")
		class AWeaponGun * Gun_B;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerWeapone")
		class AWeaponFire * FireWeapon_A;
		

	///////////////////////////////////////////////////////////////////////////////////

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wepone_IK_name")
		FName Wepone_IK_name_A;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wepone_IK_name")
		FName Wepone_IK_name_B;

	///////////////////////////////////////////////////////////////////////////////////

	UPROPERTY(VisibleAnywhere, Category = "PUBG", Meta = (AllowPrivateAccess = "true"))
		class UCurveFloat * TurnBackCurve;

	 FTimeline TurnBackTimeLine;		//用于绑定TimeLine每次更新时调用的函数

	UFUNCTION()
		void UpdateControllerRotation(float Value);

	FRotator TargetControlRotation;
	FRotator CurrentContrtolRotation;


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

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void ExamineHP();


	UFUNCTION(BlueprintCallable)
	void AttackOn();
	UFUNCTION(BlueprintCallable)
	void AttackOff();

	UFUNCTION(BlueprintCallable)
		void SprintPressed();
	UFUNCTION(BlueprintCallable)
		void SprintReleased();
	UFUNCTION(BlueprintCallable)
		void FreelookPressed();
	UFUNCTION(BlueprintCallable)
		void FreelookReleased();
	UFUNCTION(BlueprintCallable)
		void WalkPressed();
	UFUNCTION(BlueprintCallable)
		void WalkReleased();
	
};
