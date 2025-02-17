// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameStatTypes.generated.h"

UENUM(BlueprintType)
enum class EStatList : uint8
{

	ESL_None UMETA(DisplayName = "None"),
	ESL_Strength UMETA(DisplayName = "Strength"),
	ESL_Dexterity UMETA(DisplayName = "Dexterity"),
	ESL_Intelligence UMETA(DisplayName = "Intelligence")

};