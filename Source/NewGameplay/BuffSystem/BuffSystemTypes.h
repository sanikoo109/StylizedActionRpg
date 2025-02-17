// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GamePropertySystem/GamePropertyTypes.h"
#include "BuffSystemTypes.generated.h"

USTRUCT(BlueprintType)
struct FBuffTableData : public FTableRowBase
{

	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Buff")
	FText Name;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Buff")
	float Duration;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Buff")
	UTexture2D* Icon;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Buff")
	FLinearColor Color = FLinearColor::White;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Buff")
	TArray<FGamePropertyData> Props;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Buff")
	TSubclassOf<class ABuffInstance> InstanceClass;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Buff")
	int32 InitCount = 1;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Buff")
	bool bMultiplyPropsByCount = true;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Buff")
	int32 MaxCount = 1;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Buff")
	bool bDebuff;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Buff")
	FName Group;

};
