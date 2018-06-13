
#include "PlayerCharacterBase.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h" 
#include "GameFramework/CharacterMovementComponent.h"
#include "Weapon/WeaponFire.h"
#include "Weapon/WeaponGun.h"
#include "Components/InputComponent.h"
#include "Engine.h"
APlayerCharacterBase::APlayerCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;
	IsDie = false;
	MaxHP = 100;
	MinHP = 0;
	HP = MaxHP;

	SprintSpeedMax = 600;
	SprintSpeedMin = 450;
	WalkSpeedMin = 190;

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
}

void APlayerCharacterBase::UpdateControllerRotation(float Value)
{
	FRotator NewRotation = FMath::Lerp(CurrentContrtolRotation, TargetControlRotation, Value);
	Controller->SetControlRotation(NewRotation);

}

void APlayerCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	if (DefaultWeaponClass)
	{
		Gun_A = GetWorld()->SpawnActor<AWeaponGun>(DefaultWeaponClass, FVector(0, 0, 0), FRotator(0, 0, 0));

		Gun_A->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), Wepone_IK_name_A);
	}
	
	//Gun_A->Execute_Fire_Int(Gun_A,true,0.1f);  测试接口调用

	if (TurnBackCurve)
	{
		FOnTimelineFloat TurnBackTimelineCallBack;
		FOnTimelineEventStatic TurnBackTimelineFinishedCallback;	//绑定TimeLine播放完后调用的函数

		TurnBackTimelineCallBack.BindUFunction(this, TEXT("UpdateControllerRotation"));//第一个调用函数对象 第二个为调用函数名
		TurnBackTimelineFinishedCallback.BindLambda([this]() {bUseControllerRotationYaw = true; });	//结束后执行Lambda表达式 将bUseControllerRotationYaw恢复为true

		TurnBackTimeLine.AddInterpFloat(TurnBackCurve, TurnBackTimelineCallBack);	
		TurnBackTimeLine.SetTimelineFinishedFunc(TurnBackTimelineFinishedCallback);	//设置TimeLine播放完后调用TurnBackTimelineFinishedCallback
	}

}
void APlayerCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ExamineHP();

	TurnBackTimeLine.TickTimeline(DeltaTime);
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
}

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

//////////////////////////////////////////////////////////////////////////开火控制
void APlayerCharacterBase::AttackOn()
{
	if (Gun_A)
	{
		Gun_A->Execute_Fire_Int(Gun_A, true, 0.0f);
	}
}

void APlayerCharacterBase::AttackOff()
{
	if (Gun_A)
	{
		Gun_A->Execute_Fire_Int(Gun_A, false, 0.0f);
	}
}

//////////////////////////////////////////////////////////////////////////行走控制
void APlayerCharacterBase::SprintPressed()
{
	GetCharacterMovement()->MaxWalkSpeed = SprintSpeedMax;
}

void APlayerCharacterBase::SprintReleased()
{

	GetCharacterMovement()->MaxWalkSpeed = SprintSpeedMin; 
}

void APlayerCharacterBase::WalkPressed()
{
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeedMin;
}

void APlayerCharacterBase::WalkReleased()
{

	GetCharacterMovement()->MaxWalkSpeed = SprintSpeedMin; 

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

