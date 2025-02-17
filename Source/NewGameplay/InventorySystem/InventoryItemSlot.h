// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "InventorySystem/InventoryItem.h"
#include "InventoryItemSlot.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FInventoryItemSlot_OnUpdateItemSignature, AInventoryItem*, NewItem, AInventoryItem*, PrevItem);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class NEWGAMEPLAY_API UInventoryItemSlot : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryItemSlot();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintReadOnly, Category = "Inventory Item Slot")
	AInventoryItem* CurrentItem;

	UFUNCTION(BlueprintCallable, Category = "Inventory Item Slot")
	void SetItem(AInventoryItem* ItemRef);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory Item Slot")
	bool IsEmpty();

	UFUNCTION()
	void OnUpdateItem_Event(AInventoryItem* NewItem, AInventoryItem* PrevItem);

	UPROPERTY(BlueprintAssignable, Category = "Inventory Item Slot")
	FInventoryItemSlot_OnUpdateItemSignature OnUpdateItem;
		
};
