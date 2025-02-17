// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkillSystemTypes.generated.h"

USTRUCT(BlueprintType)
struct FSkillData
{

	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Skill")
	FName Skill;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Skill")
	bool bUnlocked;

};

USTRUCT(BlueprintType)
struct FSkillTableData : public FTableRowBase
{

	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Skill")
	FText Name;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Skill")
	FName NeededSkill;

};