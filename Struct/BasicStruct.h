// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasicStruct.generated.h"

USTRUCT(BlueprintType)
struct FTrench
{
	GENERATED_BODY()

		FTrench();


	UPROPERTY()
	FName TrenchName;

	UPROPERTY()
	class AWeaponBase * Weapon;



	bool IsName()
	{
		if (TrenchName == "NULL")
		{
			return false;
		}
		else
		{
			return true;
		}
	};
	bool IsName(FName name)
	{
		if (TrenchName == name)
		{
			return true;
		}
		else
		{
			return false;
		}
	};

	bool IsWeapon()
	{
		if (Weapon == nullptr)
		{
			return false;
		}
		else
		{
			return true;
		}
	};

	class AWeaponBase * GetWeapon();


	bool SetWeapon(class USkeletalMeshComponent * mesh, AWeaponBase * weapon);

};

//USTRUCT()
//struct FJoyStruct
//{
//	GENERATED_BODY()
//
//		//Always make USTRUCT variables into UPROPERTY()
//		//    any non-UPROPERTY() struct vars are not replicated
//
//		// So to simplify your life for later debugging, always use UPROPERTY()
//		UPROPERTY()
//		int32 SampleInt32;
//
//	//If you want the property to appear in BP, make sure to use this instead
//	//UPROPERTY(BlueprintReadOnly)
//	UPROPERTY()
//		AActor* TargetActor;
//
//	//Set
//	void SetInt(const int32 NewValue)
//	{
//		SampleInt32 = NewValue;
//	}
//
//	//Get
//	AActor* GetActor()
//	{
//		return TargetActor;
//	}
//
//	//Check
//	bool ActorIsValid() const
//	{
//		if (!TargetActor) return false;
//		return TargetActor->IsValidLowLevel();
//	}
//
//	//Constructor
//	FJoyStruct()
//	{
//		//Always initialize your USTRUCT variables!
//		//   exception is if you know the variable type has its own default constructor
//		SampleInt32 = 5;
//		TargetActor = NULL;
//	}
//};