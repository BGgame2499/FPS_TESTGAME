
#include "PlayerCharacterBase.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h" 
#include "GameFramework/CharacterMovementComponent.h"
#include "Weapon/WeaponFire.h"
#include "Weapon/WeaponGun.h"
#include "Weapon/WeaponBase.h"
#include "Components/InputComponent.h"
#include "Engine.h"
APlayerCharacterBase::APlayerCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;
	IsDie = false;
	IsAim = false;
	MaxHP = 100;
	MinHP = 0;
	HP = MaxHP;
	//CurrentStateEnum = PlayerStateEnum::Idle;	//角色状态枚举
	CurrentWeaponAnimStateEnum = PlayerWeaponStateEnum::GunComplete;	//武器动画枚举
	CurrentHandWeaponState = CurrentHandWeaponStateEnum::Hand;	//当前持有武器枚举
	MovementComp = GetCharacterMovement();

	Wepone_IK_name_A = "Weapon_A";	//默认名称
	Wepone_IK_name_B = "Weapon_B"; 
	Wepone_Hand_name = "HandGun_R";

	SprintSpeed = 600;
	RunSpeed = 450;
	WalkSpeed = 190;
	CrouchSpeed = 190;

	CurrentSpringLength = 300.0f;
	AimSpringLength = 150.0f;

	PlayerMeshStatic = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlayerMeshStatic"));
	PlayerMeshStatic->SetupAttachment(GetRootComponent());

	CameraBoomComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoomComp"));
	CameraBoomComp->SetupAttachment(GetRootComponent());
	CameraBoomComp->SocketOffset = FVector(0, 35, 0);
	CameraBoomComp->TargetOffset = FVector(0, 0, 55);
	CameraBoomComp->bUsePawnControlRotation = true;	//允许跟随角色旋转
	CameraBoomComp->bEnableCameraRotationLag = true;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(CameraBoomComp);

	static ConstructorHelpers::FObjectFinder<UCurveFloat> FindTurnBackCurve(TEXT("/Game/PUBG_Assent/Animation/TurnBackCurve")); //加载TurnBackCurve
		TurnBackCurve = FindTurnBackCurve.Object;

	static ConstructorHelpers::FObjectFinder<UCurveFloat> FindAimSpringCurve(TEXT("/Game/PUBG_Assent/Animation/AimSpringCurve")); //加载AimSpringCurve
		AimSpringCurve = FindAimSpringCurve.Object;

}

void APlayerCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	if (DefaultWeaponClass)
	{
		Gun_A = GetWorld()->SpawnActor<AWeaponGun>(DefaultWeaponClass, FVector(0, 0, 0), FRotator(0, 0, 0));

		Gun_A->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), Wepone_IK_name_A);

		Gun_B = GetWorld()->SpawnActor<AWeaponGun>(DefaultWeaponClass, FVector(0, 0, 0), FRotator(0, 0, 0));

		Gun_B->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), Wepone_IK_name_B);
	}
	
	//Gun_A->Execute_Fire_Int(Gun_A,true,0.1f);  测试接口调用

	if (TurnBackCurve)
	{
		FOnTimelineFloat TurnBackTimelineCallBack;
		FOnTimelineEventStatic TurnBackTimelineFinishedCallback;	//绑定TimeLine播放完后调用的函数

		TurnBackTimelineCallBack.BindUFunction(this, TEXT("UpdateControllerRotation"));//第一个调用函数对象 第二个为调用函数名
		TurnBackTimelineFinishedCallback.BindLambda([this]() {bUseControllerRotationYaw = false; });	//结束后执行Lambda表达式 将bUseControllerRotationYaw恢复为true

		TurnBackTimeLine.AddInterpFloat(TurnBackCurve, TurnBackTimelineCallBack);	
		TurnBackTimeLine.SetTimelineFinishedFunc(TurnBackTimelineFinishedCallback);	//设置TimeLine播放完后调用TurnBackTimelineFinishedCallback

	}

	if (AimSpringCurve)
	{
		FOnTimelineFloat AimSpringTimelineCallBack;

		AimSpringTimelineCallBack.BindUFunction(this, TEXT("UpdateSpringLength"));

		AimSpringTimeLine.AddInterpFloat(AimSpringCurve, AimSpringTimelineCallBack);

	}

}
void APlayerCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ExamineHP();

	TurnBackTimeLine.TickTimeline(DeltaTime); //tick中绑定TimeLine
	AimSpringTimeLine.TickTimeline(DeltaTime);

}
void APlayerCharacterBase::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this,&APlayerCharacterBase::Jump);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &APlayerCharacterBase::SprintPressed);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &APlayerCharacterBase::SprintReleased);

	PlayerInputComponent->BindAction("Freelook", IE_Pressed, this, &APlayerCharacterBase::FreelookPressed);
	PlayerInputComponent->BindAction("Freelook", IE_Released, this, &APlayerCharacterBase::FreelookReleased);

	PlayerInputComponent->BindAction("Walk", IE_Pressed, this, &APlayerCharacterBase::WalkPressed);
	PlayerInputComponent->BindAction("Walk", IE_Released, this, &APlayerCharacterBase::WalkReleased);

	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &APlayerCharacterBase::CrouchPressed);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &APlayerCharacterBase::CrouchReleased);

	PlayerInputComponent->BindAction("Aim", IE_Pressed, this, &APlayerCharacterBase::AimPressed);
	PlayerInputComponent->BindAction("Aim", IE_Released, this, &APlayerCharacterBase::AimReleased);


	PlayerInputComponent->BindAction("Weapon_1", IE_Pressed, this, &APlayerCharacterBase::Weapon_1Pressed);
	PlayerInputComponent->BindAction("Weapon_2", IE_Pressed, this, &APlayerCharacterBase::Weapon_2Pressed);
	PlayerInputComponent->BindAction("Hand", IE_Pressed, this, &APlayerCharacterBase::HandPressed);
}

//////////////////////////////////////////////////////////////////////////血量
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
//////////////////////////////////////////////////////////////////////////TimeLine调用函数
void APlayerCharacterBase::UpdateControllerRotation(float Value)
{
	FRotator NewRotation = FMath::Lerp(CurrentContrtolRotation, TargetControlRotation, Value);
	Controller->SetControlRotation(NewRotation);


}

void APlayerCharacterBase::UpdateSpringLength(float Value)
{

	float NewArmLength = FMath::Lerp(CurrentSpringLength, AimSpringLength, Value);

	CameraBoomComp->TargetArmLength = NewArmLength;

}
//////////////////////////////////////////////////////////////////////////开火控制
void APlayerCharacterBase::AttackOn()
{
	if (CurrentHandWeapon)
	{
		CurrentHandWeapon->Execute_Fire_Int(CurrentHandWeapon, true, 0.0f);
	}
	
	bUseControllerRotationYaw = true;
	
}

void APlayerCharacterBase::AttackOff()
{
	if (CurrentHandWeapon)
	{
		CurrentHandWeapon->Execute_Fire_Int(CurrentHandWeapon, false, 0.0f);
	}

	if (!IsAim)
	{
		bUseControllerRotationYaw = false;
	}
}

//////////////////////////////////////////////////////////////////////////行走控制
void APlayerCharacterBase::SprintPressed()
{
	if (!IsAim)	
	{
		MovementComp->MaxWalkSpeed = SprintSpeed;
	}
	else   
	{
		MovementComp->MaxWalkSpeed = RunSpeed;   //瞄准状态下限制最大速度
	}
}

void APlayerCharacterBase::SprintReleased()
{
	if (!IsAim)
	{
		MovementComp->MaxWalkSpeed = RunSpeed;
	}
	else
	{
		MovementComp->MaxWalkSpeed = WalkSpeed;
	}
}

void APlayerCharacterBase::WalkPressed()
{
	MovementComp->MaxWalkSpeed = WalkSpeed;
}

void APlayerCharacterBase::WalkReleased()
{

	MovementComp->MaxWalkSpeed = RunSpeed;

}
void APlayerCharacterBase::CrouchPressed()
{

	Crouch(true);
	MovementComp->MaxWalkSpeed = CrouchSpeed;
}
void APlayerCharacterBase::CrouchReleased()
{

	UnCrouch();
	MovementComp->MaxWalkSpeed = RunSpeed;

}
//////////////////////////////////////////////////////////////////////////视角控制
void APlayerCharacterBase::FreelookPressed()
{
	TargetControlRotation = GetControlRotation();
	bUseControllerRotationYaw = false;
}

void APlayerCharacterBase::FreelookReleased()
{
	CurrentContrtolRotation = GetControlRotation();
	TurnBackTimeLine.PlayFromStart();	//播放TimeLine

}
void APlayerCharacterBase::AimPressed()
{

	bUseControllerRotationYaw = true;
	AimSpringTimeLine.PlayFromStart();
	IsAim = true;

	if (CurrentHandWeaponState != CurrentHandWeaponStateEnum::Hand)		//如果现在的武器不是拳头则限制走路速度
	{
		MovementComp->MaxWalkSpeed = WalkSpeed;
	}
}

void APlayerCharacterBase::AimReleased()
{

	bUseControllerRotationYaw = false;
	AimSpringTimeLine.ReverseFromEnd();	//倒叙播放AimSpringTimeLine

	IsAim = false;
	
	MovementComp->MaxWalkSpeed = RunSpeed;
	
}

//////////////////////////////////////////////////////////////////////////武器控制
void APlayerCharacterBase::Weapon_1Pressed()
{
	if (Gun_A && CurrentHandWeaponState != CurrentHandWeaponStateEnum::Weapon_1)
	{
		CurrentWeaponAnimStateEnum = PlayerWeaponStateEnum::Take_Gun;
		CurrentHandWeaponState = CurrentHandWeaponStateEnum::Weapon_1;
	}
}
void APlayerCharacterBase::Weapon_2Pressed()
{
	if (Gun_B && CurrentHandWeaponState != CurrentHandWeaponStateEnum::Weapon_2)
	{
		CurrentWeaponAnimStateEnum = PlayerWeaponStateEnum::Take_Gun;
		CurrentHandWeaponState = CurrentHandWeaponStateEnum::Weapon_2;
	}
}

void APlayerCharacterBase::HandPressed()
{
	if (CurrentHandWeaponState != CurrentHandWeaponStateEnum::Hand)
	{
		CurrentWeaponAnimStateEnum = PlayerWeaponStateEnum::Down_Gun;
		CurrentHandWeaponState = CurrentHandWeaponStateEnum::Hand;
	}
}


void APlayerCharacterBase::UpdateWeapon()
{
	switch (CurrentHandWeaponState)
	{
	case CurrentHandWeaponStateEnum::Weapon_1:
		if (Gun_A)
		{
			if (CurrentHandWeapon)
			{
				Gun_B = Cast<AWeaponGun>(CurrentHandWeapon);
				Gun_B->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), Wepone_IK_name_B);
				CurrentHandWeapon = nullptr;
			}
			CurrentHandWeapon = Gun_A;
			CurrentHandWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), Wepone_Hand_name);
			Gun_A = nullptr;
		}
		break;
	case CurrentHandWeaponStateEnum::Weapon_2:
		if (Gun_B)
		{
			if (CurrentHandWeapon)
			{
				Gun_A = Cast<AWeaponGun>(CurrentHandWeapon);
				Gun_A->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), Wepone_IK_name_A);
				CurrentHandWeapon = nullptr;
			}
			CurrentHandWeapon = Gun_B;
			CurrentHandWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), Wepone_Hand_name);
			Gun_B = nullptr;
		}
		break;
	case CurrentHandWeaponStateEnum::Hand:
		if (Gun_A == nullptr)
		{
			Gun_A = Cast<AWeaponGun>(CurrentHandWeapon);
			Gun_A->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), Wepone_IK_name_A);
			CurrentHandWeapon = nullptr;
			break;
		}
		if (Gun_B == nullptr)
		{
			Gun_B = Cast<AWeaponGun>(CurrentHandWeapon);
			Gun_B->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), Wepone_IK_name_B);
			CurrentHandWeapon = nullptr;
			break;
		}
		break;
	default:
		break;
	}
	CurrentWeaponAnimStateEnum = PlayerWeaponStateEnum::GunComplete;
}

