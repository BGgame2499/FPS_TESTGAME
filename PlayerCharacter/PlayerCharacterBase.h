
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/TimelineComponent.h"
#include "Enum/PlayerStateEnum.h"
#include "Struct/BasicStruct.h"
#include "Interface/AddWeaponInterface/I_AddWeapon.h"
#include "PlayerCharacterBase.generated.h"

UENUM(BlueprintType)
enum class CurrentHandWeaponStateEnum : uint8
{
	Weapon_1,
	Weapon_2,
	Hand


};

UCLASS()
class FPS_TESTGAME_API APlayerCharacterBase : public ACharacter ,public II_AddWeapon
{
	GENERATED_BODY()

public:
	APlayerCharacterBase();

	//class UPawnAnimStateBase * PawnAnimState;

	//UPROPERTY(EditAnywhere,BlueprintReadWrite)
	//UStaticMeshComponent * PlayerMeshStatic;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USpringArmComponent * CameraBoomComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UCameraComponent * CameraComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USceneComponent * ThrowWeaponScene;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UHealthComponent * HealthComp;

	class UCharacterMovementComponent * MovementComp;

public:

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerState")
		//PlayerStateEnum  CurrentStateEnum;

	UPROPERTY(Replicated,EditAnywhere,BlueprintReadWrite,Category = "PlayerState")
		bool bDied;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerState")
		bool bAim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerState")
		int32 SprintSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerState")
		int32 RunSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerState")
		int32 WalkSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerState")
		int32 CrouchSpeed;

	///////////////////////////////////////////////////////////////////////////////////

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerWeapone")
		TSubclassOf<class AWeaponGun > DefaultWeaponClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "PlayerWeapone")
		class AWeaponBase * CurrentHandWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "PlayerState")
		PlayerWeaponAnimStateEnum CurrentWeaponAnimStateEnum;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Replicated, Category = "PlayerState")
		CurrentHandWeaponStateEnum CurrentHandWeaponState;

	UFUNCTION(BlueprintCallable)
		virtual void UpdateWeapon();

	UFUNCTION(BlueprintCallable,Server, Reliable, WithValidation)
		virtual void TakeWeapon(CurrentHandWeaponStateEnum HandWeaponEnum);


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerWeapone")
		int32 GunTrenchNum;	//要生成的插槽数量

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TrenchArray")
		TArray<FTrench> GunTrenchArray;	//插槽数组p-[=

	UFUNCTION(BlueprintCallable)
		virtual bool AddWeapon_Int_Implementation(class AWeaponBase * Gun);	//实现添加武器接口

	UFUNCTION(BlueprintCallable)
		virtual AWeaponBase * GetGunWeapon(int32 TrenchID);

	///////////////////////////////////////////////////////////////////////////////////


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wepone_IK_name")
		FName Wepone_Hand_name;


	///////////////////////////////////////////////////////////////////////////////////

	UPROPERTY(VisibleAnywhere, Category = "PUBG", Meta = (AllowPrivateAccess = "true"))
		class UCurveFloat * TurnBackCurve;

	 FTimeline TurnBackTimeLine;		//用于绑定TimeLine每次更新时调用的函数

	UFUNCTION()
		void UpdateControllerRotation(float Value);

	FRotator TargetControlRotation;
	FRotator CurrentContrtolRotation;


	UPROPERTY(VisibleAnywhere, Category = "PUBG", Meta = (AllowPrivateAccess = "true"))
		class UCurveFloat * AimSpringCurve;

	FTimeline AimSpringTimeLine;

	UFUNCTION()
		void UpdateSpringLength(float Value);

	float DefaultFOV;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Aim")
	float ZoomedFOV;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Aim",meta = (ClampMin = 0.1f , ClampMax = 100.0f))
	float ZoomInterSpeed;

	///////////////////////////////////////////////////////////////////////////////////

	UPROPERTY(BlueprintReadOnly, Category = "CurrentGameModeBase")
		class AFPS_TESTGAMEGameModeBase * CurrentGameModeBase;


	///////////////////////////////////////////////////////////////////////////////////

	UFUNCTION(Server, Reliable, WithValidation)
		void PlayDieSet();

private:

protected:
	virtual void BeginPlay() override;


public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual FVector GetPawnViewLocation() const override;
	virtual FRotator GetViewRotation() const override;


	UFUNCTION()
		void OnHealthChanged(UHealthComponent* HealthComponent, float Health, float HealthDelta, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
	UFUNCTION(BlueprintCallable)
	void AttackOn();
	UFUNCTION(BlueprintCallable)
	void AttackOff();

	UFUNCTION(BlueprintNativeEvent)
		void RecoilRifleFire(FVector2D RandomRecoilPith, FVector2D RandomRecoilYaw);
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
	UFUNCTION(BlueprintCallable)
		void CrouchPressed();
	UFUNCTION(BlueprintCallable)
		void CrouchReleased();
	UFUNCTION(BlueprintCallable)
		void AimPressed();
	UFUNCTION(BlueprintCallable)
		void AimReleased();

	UFUNCTION(BlueprintCallable)
		void Weapon_1Pressed();
	UFUNCTION(BlueprintCallable)
		void Weapon_2Pressed();
	UFUNCTION(BlueprintCallable)
		void HandPressed();
	UFUNCTION(BlueprintCallable)
		void ThrowWeapon();
	
};
