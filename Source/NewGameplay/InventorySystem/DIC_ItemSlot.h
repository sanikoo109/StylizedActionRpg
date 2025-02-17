// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventorySystem/DesignItemComponent.h"
#include "InventorySystem/InventorySystemTypes.h"
#include "InventorySystem/InventoryComponent.h"
#include "DIC_ItemSlot.generated.h"

UCLASS()
class NEWGAMEPLAY_API UDIC_ItemSlot : public UDesignItemComponent
{

	GENERATED_BODY()

protected:

	virtual void BeginPlay() override;

public:

	UPROPERTY(BlueprintReadOnly, Category = "DIC Item Slot")
	class UInventoryComponent* ParentInventoryComp;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "DIC Item Slot")
	EItemSlotType Slot;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "DIC Item Slot")
	bool bShowMultipleMesh;

	UPROPERTY(BlueprintReadOnly, Category = "DIC Item Slot")
	FName SavedStartSocket;

	UFUNCTION()
	void OnUpdateSelectedItem_Event(EItemSlotType InSlot, AInventoryItem* NewItem, AInventoryItem* PrevItem);
	
};
