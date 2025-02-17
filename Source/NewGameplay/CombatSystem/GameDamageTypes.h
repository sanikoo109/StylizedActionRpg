// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameDamageTypes.generated.h"

UENUM(BlueprintType)
enum class EDamageType : uint8
{

	EDT_None UMETA(DisplayName = "None"),
	EDT_Physical UMETA(DisplayName = "Physical"),
	EDT_Cold UMETA(DisplayName = "Cold"),
	EDT_Poison UMETA(DisplayName = "Poison"),
	EDT_Fire UMETA(DisplayName = "Fire"),
	EDT_Electric UMETA(DisplayName = "Electric"),
	EDT_Magical UMETA(DisplayName = "Magical"),
	EDT_MAX UMETA(Hidden)

};
ENUM_RANGE_BY_COUNT(EDamageType, EDamageType::EDT_MAX)

USTRUCT(BlueprintType)
struct FGameTypeDamage
{

	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Game Type Damage")
	EDamageType Type;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Game Type Damage")
	float Value;

};

USTRUCT(BlueprintType)
struct FGameDamage
{

	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Game Damage")
	TArray<FGameTypeDamage> TypeDamageList;

	float GetSum()
	{
		float OutValue = 0.0f;
		for (FGameTypeDamage TypeDamage : TypeDamageList)
		{
			OutValue += TypeDamage.Value;
		}
		return OutValue;
	}

};
