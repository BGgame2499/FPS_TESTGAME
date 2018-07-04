// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "I_AddWeapon.generated.h"
/**
 * 
 */
UINTERFACE(Blueprintable)
class UI_AddWeapon : public UInterface
{
	GENERATED_BODY()
	
};

class II_AddWeapon
{
	GENERATED_BODY() 

public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "AddWeaponInterface")	//BlueprintNativeEvent要有默认C++中实现
		bool AddWeapon_Int(class AWeaponBase * Weaponbase);

};