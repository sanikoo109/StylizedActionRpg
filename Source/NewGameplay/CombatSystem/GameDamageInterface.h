// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CombatSystem/GameDamageTypes.h"
#include "GameDamageInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UGameDamageInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class NEWGAMEPLAY_API IGameDamageInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Game Damage")
	void ApplyGameDamage(FGameDamage Damage, AController* EventInstigator, AActor* DamageCauser, bool bHit, FVector HitFromDirection, FHitResult HitInfo, FGameDamage& AppliedDamage);
	virtual void ApplyGameDamage_Implementation(FGameDamage Damage, AController* EventInstigator, AActor* DamageCauser, bool bHit, FVector HitFromDirection, FHitResult HitInfo, FGameDamage& AppliedDamage) {};

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Game Damage")
	void Burn();
	virtual void Burn_Implementation() {};

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Game Damage")
	void Cold();
	virtual void Cold_Implementation() {};

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Game Damage")
	void Poison();
	virtual void Poison_Implementation() {};

};
