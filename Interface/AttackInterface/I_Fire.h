// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "I_Fire.generated.h"
/**
 * 
 */
UINTERFACE(Blueprintable)
class UI_Fire : public UInterface
{
	GENERATED_BODY()
	
};

class II_Fire
{
	GENERATED_BODY() 

public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "FireInterface")	//BlueprintNativeEvent要有默认C++中实现
		bool Fire_Int(bool isFire,float Time);

	/*UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "FireInterface")  //BlueprintImplementableEventC++中声明 蓝图实现
		void TEST_Inter(bool is);*/

};