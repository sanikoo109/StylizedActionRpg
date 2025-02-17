// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CombatSystem/GameDamageTypes.h"
#include "SpellSystemTypes.generated.h"

UENUM(BlueprintType)
enum class ESpellMeshTarget : uint8
{

	ESMT_None UMETA(DisplayName = "None"),
	ESMT_Auto UMETA(DisplayName = "Auto"),
	ESMT_Char UMETA(DisplayName = "Char"),
	ESMT_Aim UMETA(DisplayName = "Aim")

};

UENUM(BlueprintType)
enum class ESpellCastAnimType : uint8
{

	ESCAT_None UMETA(DisplayName = "None"),
	ESCAT_Hand UMETA(DisplayName = "Hand"),
	ESCAT_Foot UMETA(DisplayName = "Foot")

};

USTRUCT(BlueprintType)
struct FSpellSocketData
{

	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Spell Socket Data")
	ESpellMeshTarget MeshTarget;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Spell Socket Data")
	FName Socket;

};

USTRUCT()
struct FSpellTableData : public FTableRowBase
{

	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Spell Table Data")
	FText Name;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Spell Table Data")
	UTexture2D* Image;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Spell Table Data")
	FLinearColor ImageColor = FLinearColor::White;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Spell Table Data")
	TSubclassOf<class ASpellInstance> InstanceClass;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Spell Table Data")
	FGameTypeDamage Damage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Spell Table Data")
	UParticleSystem* Effect;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Spell Table Data")
	float ManaCost;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Spell Table Data")
	ESpellCastAnimType CastAnimType = ESpellCastAnimType::ESCAT_Hand;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Spell Table Data")
	float Radius = 25.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Spell Table Data")
	FSpellSocketData SocketName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Spell Table Data")
	FSpellSocketData RotationSocketName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Spell Table Data")
	FSpellSocketData PreviewSocketName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Spell Table Data")
	float Duration;

};