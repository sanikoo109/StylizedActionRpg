// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameAttributeTypes.generated.h"

UENUM(BlueprintType)
enum class EAttributeList : uint8
{

	EAL_None UMETA(DisplayName = "None"),
	EAL_Health UMETA(DisplayName = "Health"),
	EAL_Mana UMETA(DisplayName = "Mana"),
	EAL_Stamina UMETA(DisplayName = "Stamina"),
	EAL_Air UMETA(DisplayName = "Air"),

};

USTRUCT(BlueprintType)
struct FAttributeData
{

	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Attribute Data")
	EAttributeList Attribute;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Attribute Data")
	float CurrentValue;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Attribute Data")
	float MaxValue;

};