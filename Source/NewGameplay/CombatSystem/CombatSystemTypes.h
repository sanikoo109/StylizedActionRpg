// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CombatSystem/GameDamageTypes.h"
#include "SpellSystem/SpellSystemTypes.h"
#include "CombatSystemTypes.generated.h"

USTRUCT(BlueprintType)
struct FCombatMoveTableData : public FTableRowBase
{

	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Combat Move")
	UAnimMontage* Montage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Combat Move")
	UAnimMontage* ItemMontage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Combat Move")
	float PlayRate = 1.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Combat Move")
	int32 MoveCount = 1;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Combat Move")
	bool bAim = true;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Combat Move")
	bool bCanInAir = true;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Combat Move")
	bool bCanReleaseAction;

};

USTRUCT(BlueprintType)
struct FCombatAttackData
{

	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Combat Attack")
	bool bDeferred;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Combat Attack")
	bool bTraceL;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Combat Attack")
	bool bTraceR;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Combat Attack")
	TArray<AActor*> DamageRegister;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Combat Attack")
	int32 MoveCount;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Combat Attack")
	UAnimMontage* CurrentMontage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Combat Attack")
	FName SpellToCast;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Combat Attack")
	FName DeferredSpell;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Combat Attack")
	ESpellCastAnimType CastAnimType;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Combat Attack")
	FName CurrentMove;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Combat Attack")
	FName DeferredMove;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Combat Attack")
	bool bShowAmmo;

};