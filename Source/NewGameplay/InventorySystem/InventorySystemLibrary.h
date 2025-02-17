// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "InventorySystem/InventorySystemTypes.h"
#include "InventorySystemLibrary.generated.h"

/**
 * 
 */
UCLASS()
class NEWGAMEPLAY_API UInventorySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory System Library")
	static bool ItemSlot_IsSecondary(EItemSlotType Slot);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory System Library")
	static bool ItemSlot_IsWeapon(EItemSlotType Slot);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory System Library")
	static bool ItemSlot_IsLeft(EItemSlotType Slot);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory System Library")
	static EItemSlotType GetWeaponSlot(bool bLeft, bool bSecondary);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory System Library")
	static EItemSlotType ItemSlot_GetPrimary(EItemSlotType Slot);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory System Library")
	static TSubclassOf<class AInventoryItem> InventoryItemTableData_GetItemClass(FInventoryItemTableData TableData);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory System Library")
	static TSubclassOf<AActor> InventoryItemTableData_GetItemDesignClass(FInventoryItemTableData TableData);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory System Library")
	static TSubclassOf<class APickUp> InventoryItemTableData_GetPickUpClass(FInventoryItemTableData TableData);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory System Library")
	static bool UseRelativeData_Equal(FUseRelativeData A, FUseRelativeData B);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory System Library")
	static bool UseRelativeData_NotEqual(FUseRelativeData A, FUseRelativeData B);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory System Library")
	static EItemSlotType ItemSlot_GetOpposite(EItemSlotType Slot);
	
};
