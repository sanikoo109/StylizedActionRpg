// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "InventorySystemDeveloperSettings.generated.h"

/**
 * 
 */
UCLASS(Config = Game, DefaultConfig)
class NEWGAMEPLAY_API UInventorySystemDeveloperSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:

	UPROPERTY(Config, EditAnywhere, Category = "Inventory System Settings")
	TSubclassOf<class AInventoryItem> DefaultItemClass;

	UPROPERTY(Config, EditAnywhere, Category = "Inventory System Settings")
	TSubclassOf<AActor> DefaultItemDesignClass;

	UPROPERTY(Config, EditAnywhere, Category = "Inventory System Settings")
	TSubclassOf<class APickUp> DefaultPickUpClass;

	UPROPERTY(Config, EditAnywhere, Category = "Inventory System Settings", meta = (AllowedClasses = "/Script/Engine.DataTable"))
	FSoftObjectPath ItemsTable;
	
};
