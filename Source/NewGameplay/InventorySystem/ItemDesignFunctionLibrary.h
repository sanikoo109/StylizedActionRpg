// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ItemDesignFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class NEWGAMEPLAY_API UItemDesignFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "Item Design Function Library")
	static void ItemDesign_SetVisible(AActor* Target, bool bVisible);

	UFUNCTION(BlueprintCallable, Category = "Item Design Function Library")
	static void ItemDesign_UpdateVisible(AActor* Target);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Item Design Function Library")
	static class AInventoryItem* ItemDesign_GetParentItem(AActor* Target);

	UFUNCTION(BlueprintCallable, Category = "Item Design Function Library")
	static void ItemDesign_SetParentItem(AActor* Target, class AInventoryItem* Item);

	UFUNCTION(BlueprintCallable, Category = "Item Design Function Library")
	static void ItemDesign_SetShowMultipleMesh(AActor* Target, bool Value);
	
};
