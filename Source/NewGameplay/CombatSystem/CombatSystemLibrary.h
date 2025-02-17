// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CombatSystem/GameDamageTypes.h"
#include "CombatSystemLibrary.generated.h"

/**
 * 
 */
UCLASS()
class NEWGAMEPLAY_API UCombatSystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Combat System Library")
	static float GetDamageOfType(FGameDamage Data, EDamageType Type);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Combat System Library")
	static float GetDamageSum(FGameDamage Data);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Combat System Library")
	static FGameDamage ScaleDamage(FGameDamage Damage, float Scale);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Game Property System Library")
	static FName DamageType_GetDamageProp(EDamageType DamageType);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Game Property System Library")
	static FName DamageType_GetDefenseProp(EDamageType DamageType);

};
