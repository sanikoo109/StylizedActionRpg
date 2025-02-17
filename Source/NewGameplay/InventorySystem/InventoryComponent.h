// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventorySystem/InventorySystemTypes.h"
#include "InventorySystem/InventoryItem.h"
#include "InventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FInventoryComponent_OnUpdateSlotItemSignature, EItemSlotType, Slot, AInventoryItem*, NewItem, AInventoryItem*, PrevItem);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FInventoryComponent_OnAddItemToGridSignature, AInventoryItem*, Item, FIntPoint, Position);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FInventoryComponent_OnRemoveItemFromGridSignature, AInventoryItem*, Item, FIntPoint, Position);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FInventoryComponent_OnUpdateDragItemSignature, AInventoryItem*, NewItem, AInventoryItem*, PrevItem);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FInventoryComponent_OnUpdateGoldItemSignature, AInventoryItem*, NewItem, AInventoryItem*, PrevItem);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInventoryComponent_OnAddItemSignature, AInventoryItem*, Item);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInventoryComponent_OnRemoveItemSignature, AInventoryItem*, Item);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInventoryComponent_OnOpenUpdateSignature, bool, bOpen);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FInventoryComponent_OnUpdateInspectInventorySignature, UInventoryComponent*, NewInventory, UInventoryComponent*, PrevInventory);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInventoryComponent_OnSelectWeaponSignature, bool, IsSecond);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FInventoryComponent_OnUpdateSelectedItemSignature, EItemSlotType, Slot, AInventoryItem*, NewItem, AInventoryItem*, PrevItem);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FInventoryComponent_OnUpdateUseRelativeItemSignature, AInventoryItem*, NewItem, AInventoryItem*, PrevItem);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FInventoryComponent_OnUpdateUseRelativeDataSignature, FUseRelativeData, NewData, FUseRelativeData, PrevData);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class NEWGAMEPLAY_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//PROP

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Inventory Component")
	FIntPoint GridSize;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Inventory Component")
	bool bUnbound;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Inventory Component")
	FIntPoint UnboundMinSize;

	UPROPERTY(BlueprintReadOnly, Category = "Inventory Component")
	TArray<AInventoryItem*> GridItems;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Inventory Component")
	TArray<FItemSlot> Slots;

	UPROPERTY(EditAnywhere, Category = "Inventory Component")
	TMap<EItemSlotType, EInventoryItemType> EnabledItemBySlot;

	UPROPERTY(BlueprintReadOnly, Category = "Inventory Component")
	AInventoryItem* CurrentDragItem;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Inventory Component")
	bool bEnableGold;

	UPROPERTY(BlueprintReadOnly, Category = "Inventory Component")
	AInventoryItem* GoldItemRef;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Inventory Component")
	TSubclassOf<class UWB_InventoryBase> InventoryWidget;

	UPROPERTY(BlueprintReadOnly, Category = "Inventory Component")
	bool IsOpen;

	UPROPERTY(BlueprintReadOnly, Category = "Inventory Component")
	UInventoryComponent* InspectingInventory;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory Component")
	FText InventoryName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory Component")
	TArray<FChestItemData> StartItems;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Inventory Component")
	bool bSecondWeapon;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Inventory Component")
	bool bTraderInventory;

	UPROPERTY(BlueprintReadOnly, Category = "Inventory Component")
	FUseRelativeData UseRelativeData;

	//FUNC

	UFUNCTION()
	int32 FindSlotID(EItemSlotType Slot);

	UFUNCTION(BlueprintCallable, Category = "Inventory Component|Slot")
	FItemSlot GetSlotData(EItemSlotType Slot, bool& bValid);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory Component|Slot")
	AInventoryItem* GetSlotItem(EItemSlotType Slot);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory Component|Slot")
	AInventoryItem* GetSelectedSlotItem(EItemSlotType Slot);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory Component|Slot")
	bool CanSetItemToSlot(EItemSlotType Slot, AInventoryItem* Item);

	UFUNCTION(BlueprintCallable, Category = "Inventory Component|Slot")
	bool SetSlotItem(EItemSlotType Slot, AInventoryItem* Item);

	UFUNCTION()
	void SetSlotItem_Fast(EItemSlotType Slot, int32 SlotID, AInventoryItem* Item);

	UFUNCTION(BlueprintCallable, Category = "Inventory Component|Slot")
	bool AddItemToSlots(AInventoryItem* Item);

	UFUNCTION(BlueprintCallable, Category = "Inventory Component|Slot")
	void RemoveItemFromSlots(AInventoryItem* Item);

	UFUNCTION(BlueprintCallable, Category = "Inventory Component|Slot")
	bool AddNewItemToSlots(AInventoryItem* Item);

	UFUNCTION(BlueprintCallable, Category = "Inventory Component|Slot")
	bool CompleteItemToSlots(AInventoryItem* Item);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory Component|Slot")
	bool IsEnableSlotItem(EItemSlotType Slot, AInventoryItem* Item);

	UFUNCTION()
	void OnUpdateSlotItem_Event(EItemSlotType Slot, AInventoryItem* NewItem, AInventoryItem* PrevItem);

	UFUNCTION(BlueprintCallable, Category = "Inventory Component|Slot")
	void SelectWeapon(bool bSecond);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory Component|Slot")
	bool IsSelectedSlot(EItemSlotType Slot);

	UFUNCTION()
	void OnSelectWeapon_Event(bool IsSecond);

	UFUNCTION()
	void CheckWeaponItem(bool bLeft);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory Component|Slot")
	EItemSlotType GetSelectedSlot(EItemSlotType Slot);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory Component|Slot")
	bool IsSlotBlocked(EItemSlotType Slot);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory Component|Slot")
	bool IsEmptySlot(EItemSlotType Slot);

	UFUNCTION(BlueprintCallable, Category = "Inventory Component|Grid")
	bool AddItemToGrid(AInventoryItem* Item);

	UFUNCTION(BlueprintCallable, Category = "Inventory Component|Grid")
	bool AddItemToGridByPosition(AInventoryItem* Item, FIntPoint Position);

	UFUNCTION()
	void AddItemToGridByPosition_Fast(AInventoryItem* Item, FIntPoint Position);

	UFUNCTION(BlueprintCallable, Category = "Inventory Component|Grid")
	void RemoveItemFromGrid(AInventoryItem* Item);

	UFUNCTION(BlueprintCallable, Category = "Inventory Component|Grid")
	bool AddNewItemToGrid(AInventoryItem* Item);

	UFUNCTION(BlueprintCallable, Category = "Inventory Component|Grid")
	bool CompleteItemToGrid(AInventoryItem* Item);

	UFUNCTION(BlueprintCallable, Category = "Inventory Component|Grid")
	TArray<FIntPoint> FindGridValidCells();

	UFUNCTION(BlueprintCallable, Category = "Inventory Component|Grid")
	TArray<FIntPoint> FindGridFreeCells();

	UFUNCTION(BlueprintCallable, Category = "Inventory Component|Grid")
	bool FindGridPositionForItem(FIntPoint ItemSize, FIntPoint& OutPosition);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory Component|Grid")
	bool IsFreeGridPositionForItem(FIntPoint ItemSize, FIntPoint Position, TArray<FIntPoint> FreeCells);

	UFUNCTION()
	void OnAddItemToGrid_Event(AInventoryItem* Item, FIntPoint Position);

	UFUNCTION()
	void OnRemoveItemFromGrid_Event(AInventoryItem* Item, FIntPoint Position);

	UFUNCTION(BlueprintCallable, Category = "Inventory Component|Grid")
	TArray<AInventoryItem*> GetOverlappingItems(FIntPoint ItemSize, FIntPoint Position, int32 ItemLimit = -1);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory Component|Grid")
	bool IsOverlappingItem(FIntPoint ItemSize_A, FIntPoint ItemPos_A, FIntPoint ItemSize_B, FIntPoint ItemPos_B);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory Component|Gold")
	int32 GetGoldCount();

	UFUNCTION(BlueprintCallable, Category = "Inventory Component|Gold")
	void SetGoldCount(int32 InCount);

	UFUNCTION(BlueprintCallable, Category = "Inventory Component|Gold")
	void AddGoldCount(int32 InCount);

	UFUNCTION(BlueprintCallable, Category = "Inventory Component|Gold")
	bool SetGoldItem(AInventoryItem* Item);

	UFUNCTION(BlueprintCallable, Category = "Inventory Component|Gold")
	bool AddGoldItem(AInventoryItem* Item);

	UFUNCTION()
	void OnUpdateGoldItem_Event(AInventoryItem* NewItem, AInventoryItem* PrevItem);

	UFUNCTION(BlueprintCallable, Category = "Inventory Component|Drag")
	void DragItem(AInventoryItem* Item);

	UFUNCTION()
	void SetDragItem(AInventoryItem* Item);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory Component|Drag")
	bool IsDragging();

	UFUNCTION(BlueprintCallable, Category = "Inventory Component|Drag")
	void DropItemToGrid(UInventoryComponent* TargetInventory, FIntPoint Position);

	UFUNCTION(BlueprintCallable, Category = "Inventory Component|Drag")
	void DropItemToSlot(UInventoryComponent* TargetInventory, EItemSlotType Slot);

	UFUNCTION(BlueprintCallable, Category = "Inventory Component|Drag")
	void DropItemToFloor(AInventoryItem* Item);

	UFUNCTION()
	void OnUpdateDragItem_Event(AInventoryItem* NewItem, AInventoryItem* PrevItem);

	UFUNCTION(BlueprintCallable, Category = "Inventory Component|Inspect")
	void InspectInventory(UInventoryComponent* InventoryRef);

	UFUNCTION(BlueprintCallable, Category = "Inventory Component|Inspect")
	void SetInspectInventory(UInventoryComponent* InventoryRef);

	UFUNCTION(BlueprintCallable, Category = "Inventory Component|Use Relative")
	void SetUseRelativeItem(EUseRelativeActionList Action, AInventoryItem* Item);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory Component|Use Relative")
	bool IsRelativeUsing();

	UFUNCTION()
	void OnUpdateUseRelativeData_Event(FUseRelativeData NewData, FUseRelativeData PrevData);

	UFUNCTION()
	void UseRelativeItem_OnEndPlay_Event(AActor* Actor, EEndPlayReason::Type EndPlayReason);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Inventory Component|Use Relative")
	void UseRelative(AInventoryItem* TargetItem);
	virtual void UseRelative_Implementation(AInventoryItem* TargetItem);

	UFUNCTION(BlueprintCallable, Category = "Inventory Component|Use Relative")
	bool Forge(AInventoryItem* Item);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory Component|Use Relative")
	bool IsEnoughMaterialsToForge(AInventoryItem* Item);

	UFUNCTION(BlueprintCallable, Category = "Inventory Component")
	void SetOpen(bool bOpen);

	UFUNCTION(BlueprintCallable, Category = "Inventory Component")
	bool AddItemRef(AInventoryItem* Item);

	UFUNCTION(BlueprintCallable, Category = "Inventory Component")
	void RemoveItemCount(FName Item, int32 Count);

	UFUNCTION(BlueprintCallable, Category = "Inventory Component")
	void RemoveItem(AInventoryItem* Item);

	UFUNCTION()
	void OnAddItem_Event(AInventoryItem* Item);

	UFUNCTION()
	void OnRemoveItem_Event(AInventoryItem* Item);

	UFUNCTION()
	void Item_OnRemoveFromParent_Event(AInventoryItem* Item);

	UFUNCTION()
	void OnOpenUpdate_Event(bool bOpen);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory Component")
	bool HaveItem(FName Item);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory Component")
	bool HaveItemRef(AInventoryItem* Item);

	UFUNCTION(BlueprintCallable, Category = "Inventory Component")
	void AddItem(FName Item, int32 Count = 1);

	UFUNCTION()
	void CreateStartItems();

	UFUNCTION(BlueprintCallable, Category = "Inventory Component")
	AInventoryItem* GetItem(FName Item);

	UFUNCTION(BlueprintCallable, Category = "Inventory Component")
	void BuyItem(AInventoryItem* Item);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory Component")
	int32 GetItemCount(FName Item);

	UFUNCTION(BlueprintCallable, Category = "Inventory Component")
	void SellItem(AInventoryItem* Item);

	//DELEGATES

	UPROPERTY(BlueprintAssignable, Category = "Inventory Component")
	FInventoryComponent_OnUpdateSlotItemSignature OnUpdateSlotItem;

	UPROPERTY(BlueprintAssignable, Category = "Inventory Component")
	FInventoryComponent_OnAddItemToGridSignature OnAddItemToGrid;

	UPROPERTY(BlueprintAssignable, Category = "Inventory Component")
	FInventoryComponent_OnRemoveItemFromGridSignature OnRemoveItemFromGrid;

	UPROPERTY(BlueprintAssignable, Category = "Inventory Component")
	FInventoryComponent_OnUpdateDragItemSignature OnUpdateDragItem;

	UPROPERTY(BlueprintAssignable, Category = "Inventory Component")
	FInventoryComponent_OnUpdateGoldItemSignature OnUpdateGoldItem;

	UPROPERTY(BlueprintAssignable, Category = "Inventory Component")
	FInventoryComponent_OnAddItemSignature OnAddItem;

	UPROPERTY(BlueprintAssignable, Category = "Inventory Component")
	FInventoryComponent_OnRemoveItemSignature OnRemoveItem;

	UPROPERTY(BlueprintAssignable, Category = "Inventory Component")
	FInventoryComponent_OnOpenUpdateSignature OnOpenUpdate;

	UPROPERTY(BlueprintAssignable, Category = "Inventory Component")
	FInventoryComponent_OnUpdateInspectInventorySignature OnUpdateInspectInventory;

	UPROPERTY(BlueprintAssignable, Category = "Inventory Component")
	FInventoryComponent_OnSelectWeaponSignature OnSelectWeapon;

	UPROPERTY(BlueprintAssignable, Category = "Inventory Component")
	FInventoryComponent_OnUpdateSelectedItemSignature OnUpdateSelectedItem;

	UPROPERTY(BlueprintAssignable, Category = "Inventory Component")
	FInventoryComponent_OnUpdateUseRelativeItemSignature OnUpdateUseRelativeItem;

	UPROPERTY(BlueprintAssignable, Category = "Inventory Component")
	FInventoryComponent_OnUpdateUseRelativeDataSignature OnUpdateUseRelativeData;

protected:

	UPROPERTY()
	UDataTable* ItemsTable;
		
};
