// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WB_InventoryBase.generated.h"

/**
 * 
 */
UCLASS()
class NEWGAMEPLAY_API UWB_InventoryBase : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly, Category = "Inventory Widget", meta = (ExposeOnSpawn = "true"))
	class UInventoryComponent* ParentInventory;
	
};
