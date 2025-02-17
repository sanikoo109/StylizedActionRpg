// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSystem/GameAttributeTypes.h"
#include "CombatSystem/CombatSystemTypes.h"
#include "StatSystem/GameStatTypes.h"
#include "GamePropertyTypes.generated.h"

UENUM(BlueprintType)
enum class EGamePropertyCategoryList : uint8
{

	EGPCL_None UMETA(DisplayName = "None"),
	EGPCL_AttributeMax UMETA(DisplayName = "Attribute Max"),
	EGPCL_AttributeRegen UMETA(DisplayName = "Attribute Regen"),
	EGPCL_Damage UMETA(DisplayName = "Damage"),
	EGPCL_Defense UMETA(DisplayName = "Defense"),
	EGPCL_Stat UMETA(DisplayName = "Stat"),
	EGPCL_StatusEffect UMETA(DisplayName = "Status Effect"),
	EGPCL_Speed UMETA(DisplayName = "Speed")

};

USTRUCT(BlueprintType)
struct FGamePropertyData
{

	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Game Property")
	FName Name;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Game Property")
	float Value;

};

USTRUCT(BlueprintType)
struct FGamePropertyTableData : public FTableRowBase
{

	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Game Property")
	EGamePropertyCategoryList Category;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Game Property")
	FText Name;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Game Property")
	EAttributeList Attribute;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Game Property")
	EDamageType DamageType;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Game Property")
	EStatList Stat;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Game Property")
	bool bDebuff;

};