// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySystem/InventoryComponent.h"
#include "InventorySystem/InventorySystemLibrary.h"
#include "InventorySystem/InventorySystemDeveloperSettings.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "InventorySystem/PickUp.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	ItemsTable = Cast<UDataTable>(GetDefault<UInventorySystemDeveloperSettings>()->ItemsTable.TryLoad());

	OnAddItemToGrid.AddDynamic(this, &UInventoryComponent::OnAddItemToGrid_Event);
	OnRemoveItemFromGrid.AddDynamic(this, &UInventoryComponent::OnRemoveItemFromGrid_Event);
	OnUpdateSlotItem.AddDynamic(this, &UInventoryComponent::OnUpdateSlotItem_Event);
	OnUpdateGoldItem.AddDynamic(this, &UInventoryComponent::OnUpdateGoldItem_Event);
	OnUpdateDragItem.AddDynamic(this, &UInventoryComponent::OnUpdateDragItem_Event);
	OnAddItem.AddDynamic(this, &UInventoryComponent::OnAddItem_Event);
	OnRemoveItem.AddDynamic(this, &UInventoryComponent::OnRemoveItem_Event);
	OnOpenUpdate.AddDynamic(this, &UInventoryComponent::OnOpenUpdate_Event);
	OnSelectWeapon.AddDynamic(this, &UInventoryComponent::OnSelectWeapon_Event);
	OnUpdateUseRelativeData.AddDynamic(this, &UInventoryComponent::OnUpdateUseRelativeData_Event);

	CreateStartItems();

}

// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

int32 UInventoryComponent::FindSlotID(EItemSlotType Slot)
{
	for (int i = 0; i < Slots.Num(); ++i)
	{
		if (Slots[i].Type == Slot)return i;
	}

	return -1;
}

FItemSlot UInventoryComponent::GetSlotData(EItemSlotType Slot, bool& bValid)
{
	bValid = false;

	int32 ID = FindSlotID(Slot);
	if (ID >= 0)
	{
		bValid = true;
		return Slots[ID];
	}
	return FItemSlot();
}

AInventoryItem* UInventoryComponent::GetSlotItem(EItemSlotType Slot)
{
	bool bValid;
	return GetSlotData(Slot, bValid).CurrentItem;
}

AInventoryItem* UInventoryComponent::GetSelectedSlotItem(EItemSlotType Slot)
{
	return GetSlotItem(GetSelectedSlot(Slot));
}

bool UInventoryComponent::CanSetItemToSlot(EItemSlotType Slot, AInventoryItem* Item)
{
	int32 SlotID = FindSlotID(Slot);
	if (SlotID < 0 || Slots[SlotID].CurrentItem == Item)return false;
	if (Item)
	{
		return (IsEnableSlotItem(Slot, Item));
	}
	else return true;
}

bool UInventoryComponent::SetSlotItem(EItemSlotType Slot, AInventoryItem* Item)
{
	int32 SlotID = FindSlotID(Slot);
	if (SlotID < 0 || Slots[SlotID].CurrentItem == Item)return false;
	if (Item)
	{
		if (!IsEnableSlotItem(Slot, Item))return false;
	}

	SetSlotItem_Fast(Slot, SlotID, Item);
	return true;
}

void UInventoryComponent::SetSlotItem_Fast(EItemSlotType Slot, int32 SlotID, AInventoryItem* Item)
{
	AInventoryItem* PrevItem = Slots[SlotID].CurrentItem;
	if (Item)Item->RemoveFromParent();
	Slots[SlotID].CurrentItem = Item;
	OnUpdateSlotItem.Broadcast(Slot, Item, PrevItem);
}

bool UInventoryComponent::AddItemToSlots(AInventoryItem* Item)
{
	if (CompleteItemToSlots(Item))return true;
	if (AddNewItemToSlots(Item))return true;
	return false;
}

void UInventoryComponent::RemoveItemFromSlots(AInventoryItem* Item)
{
	for (FItemSlot LocalSlot : Slots)
	{
		if (LocalSlot.CurrentItem == Item)SetSlotItem(LocalSlot.Type, nullptr);
	}
}

bool UInventoryComponent::AddNewItemToSlots(AInventoryItem* Item)
{
	for (FItemSlot LocalSlot : Slots)
	{
		if (IsSelectedSlot(LocalSlot.Type) && LocalSlot.CurrentItem == nullptr)
		{
			if (SetSlotItem(LocalSlot.Type, Item))return true;
		}
	}
	return false;
}

bool UInventoryComponent::CompleteItemToSlots(AInventoryItem* Item)
{
	if (!Item)return false;
	for (FItemSlot LocalSlot : Slots)
	{
		if (LocalSlot.CurrentItem)
		{
			if (LocalSlot.CurrentItem->CompleteItem(Item))return true;
		}
	}
	return false;
}

bool UInventoryComponent::IsEnableSlotItem(EItemSlotType Slot, AInventoryItem* Item)
{
	if (!Item)return false;

	if (EnabledItemBySlot.FindRef(Slot) == Item->ItemTableData.Type)
	{
		if (!IsSlotBlocked(Slot))
		{
			if (Slot == EItemSlotType::EIST_WeaponR || Slot == EItemSlotType::EIST_WeaponR2)
			{
				if (!Item->ItemTableData.bEnableRightArm)return false;
			}
			if (Slot == EItemSlotType::EIST_WeaponL || Slot == EItemSlotType::EIST_WeaponL2)
			{
				if (!Item->ItemTableData.bEnableLeftArm)return false;
			}
			return true;
		}
	}

	EItemSlotType OppositeSlot = UInventorySystemLibrary::ItemSlot_GetOpposite(Slot);
	if (AInventoryItem* OppositeItem = GetSlotItem(OppositeSlot))
	{
		if (OppositeItem->ItemTableData.Ammo == Item->ItemRowName)
		{
			return true;
		}
	}
	return false;
}

void UInventoryComponent::OnUpdateSlotItem_Event(EItemSlotType Slot, AInventoryItem* NewItem, AInventoryItem* PrevItem)
{
	if (PrevItem)
	{
		PrevItem->SetupParent(FInventoryItemParentData());
		OnRemoveItem.Broadcast(PrevItem);
	}
	if (NewItem)
	{
		FInventoryItemParentData ParentData;

		ParentData.Type = EInventoryItemParentType::EIIPT_InventorySlot;
		ParentData.Inventory = this;
		ParentData.InventorySlot = Slot;

		NewItem->SetupParent(ParentData);
		OnAddItem.Broadcast(NewItem);
	}

	if (IsSelectedSlot(Slot))
	{
		EItemSlotType PrimarySlot = UInventorySystemLibrary::ItemSlot_GetPrimary(Slot);
		OnUpdateSelectedItem.Broadcast(PrimarySlot, NewItem, PrevItem);
	}
}

void UInventoryComponent::SelectWeapon(bool bSecond)
{
	if (bSecondWeapon != bSecond)
	{
		bSecondWeapon = bSecond;
		OnSelectWeapon.Broadcast(bSecondWeapon);
	}
}

bool UInventoryComponent::IsSelectedSlot(EItemSlotType Slot)
{
	return GetSelectedSlot(Slot) == Slot;
}

void UInventoryComponent::OnSelectWeapon_Event(bool IsSecond)
{
	CheckWeaponItem(false);
	CheckWeaponItem(true);
}

void UInventoryComponent::CheckWeaponItem(bool bLeft)
{
	EItemSlotType PrimarySlot = UInventorySystemLibrary::GetWeaponSlot(bLeft, false);
	EItemSlotType SecondarySlot = UInventorySystemLibrary::GetWeaponSlot(bLeft, true);

	AInventoryItem* PrevItem = GetSlotItem(bSecondWeapon ? PrimarySlot : SecondarySlot);
	AInventoryItem* NewItem = GetSlotItem(bSecondWeapon ? SecondarySlot : PrimarySlot);

	if (NewItem != PrevItem)
	{
		OnUpdateSelectedItem.Broadcast(PrimarySlot, NewItem, PrevItem);
	}
}

EItemSlotType UInventoryComponent::GetSelectedSlot(EItemSlotType Slot)
{
	if (UInventorySystemLibrary::ItemSlot_IsWeapon(Slot))
	{
		bool IsLeftSlot = UInventorySystemLibrary::ItemSlot_IsLeft(Slot);
		return UInventorySystemLibrary::GetWeaponSlot(IsLeftSlot, bSecondWeapon);
	}

	return Slot;
}

bool UInventoryComponent::IsSlotBlocked(EItemSlotType Slot)
{
	EItemSlotType OppositeSlot = UInventorySystemLibrary::ItemSlot_GetOpposite(Slot);
	if (AInventoryItem* OppositeItem = GetSlotItem(OppositeSlot))
	{
		if (OppositeItem->ItemTableData.bTwoHanded)return true;
	}

	return false;
}

bool UInventoryComponent::IsEmptySlot(EItemSlotType Slot)
{
	bool bValid;
	AInventoryItem* ItemRef = GetSlotData(Slot, bValid).CurrentItem;
	return !ItemRef;
}

bool UInventoryComponent::AddItemToGrid(AInventoryItem* Item)
{
	if (CompleteItemToGrid(Item))return true;
	if (AddNewItemToGrid(Item))return true;
	return false;
}

bool UInventoryComponent::AddItemToGridByPosition(AInventoryItem* Item, FIntPoint Position)
{
	if (!Item)return false;

	TArray<FIntPoint> FreeCells = FindGridFreeCells();
	if (IsFreeGridPositionForItem(Item->ItemTableData.Size, Position, FreeCells))
	{
		AddItemToGridByPosition_Fast(Item, Position);
		return true;
	}
	return false;
}

void UInventoryComponent::AddItemToGridByPosition_Fast(AInventoryItem* Item, FIntPoint Position)
{
	Item->RemoveFromParent();
	GridItems.Add(Item);
	Item->Position = Position;
	OnAddItemToGrid.Broadcast(Item, Position);
}

void UInventoryComponent::RemoveItemFromGrid(AInventoryItem* Item)
{
	if (Item && GridItems.Contains(Item))
	{
		FIntPoint SavedPosition = Item->Position;
		GridItems.Remove(Item);
		OnRemoveItemFromGrid.Broadcast(Item, SavedPosition);
	}
}

bool UInventoryComponent::AddNewItemToGrid(AInventoryItem* Item)
{
	if (!Item)return false;
	FIntPoint FindedPos;
	if (FindGridPositionForItem(Item->ItemTableData.Size, FindedPos))
	{
		AddItemToGridByPosition_Fast(Item, FindedPos);
		return true;
	}
	return false;
}

bool UInventoryComponent::CompleteItemToGrid(AInventoryItem* Item)
{
	for (AInventoryItem* LocalItem : GridItems)
	{
		if (LocalItem->CompleteItem(Item))return true;
	}
	return false;
}

TArray<FIntPoint> UInventoryComponent::FindGridValidCells()
{
	TArray<FIntPoint> OutCells;
	for (int y = 0; y < GridSize.Y; ++y)
	{
		for (int x = 0; x < GridSize.X; ++x)
		{
			OutCells.Add(FIntPoint(x, y));
		}
	}
	return OutCells;
}

TArray<FIntPoint> UInventoryComponent::FindGridFreeCells()
{
	TArray<FIntPoint> OutCells = FindGridValidCells();

	for (AInventoryItem* LocalItem : GridItems)
	{
		for (int y = 0; y < LocalItem->ItemTableData.Size.Y; ++y)
		{
			for (int x = 0; x < LocalItem->ItemTableData.Size.X; ++x)
			{
				FIntPoint CurrentPos = FIntPoint(x, y) + LocalItem->Position;
				OutCells.Remove(CurrentPos);
			}
		}
	}

	return OutCells;
}

bool UInventoryComponent::FindGridPositionForItem(FIntPoint ItemSize, FIntPoint& OutPosition)
{
	OutPosition = FIntPoint(0, 0);
	TArray<FIntPoint> FreeCells = FindGridFreeCells();
	for (FIntPoint Pos : FreeCells)
	{
		if (IsFreeGridPositionForItem(ItemSize, Pos, FreeCells))
		{
			OutPosition = Pos;
			return true;
		}
	}
	return false;
}

bool UInventoryComponent::IsFreeGridPositionForItem(FIntPoint ItemSize, FIntPoint Position, TArray<FIntPoint> FreeCells)
{
	for (int y = 0; y < ItemSize.Y; ++y)
	{
		for (int x = 0; x < ItemSize.X; ++x)
		{
			FIntPoint CurrentPos = FIntPoint(x, y) + Position;
			if (!FreeCells.Contains(CurrentPos))return false;
		}
	}
	return true;
}

void UInventoryComponent::OnAddItemToGrid_Event(AInventoryItem* Item, FIntPoint Position)
{
	FInventoryItemParentData ParentData;

	ParentData.Type = EInventoryItemParentType::EIIPT_InventoryGrid;
	ParentData.Inventory = this;

	Item->SetupParent(ParentData);
	OnAddItem.Broadcast(Item);
}

void UInventoryComponent::OnRemoveItemFromGrid_Event(AInventoryItem* Item, FIntPoint Position)
{
	Item->SetupParent(FInventoryItemParentData());
	OnRemoveItem.Broadcast(Item);
}

TArray<AInventoryItem*> UInventoryComponent::GetOverlappingItems(FIntPoint ItemSize, FIntPoint Position, int32 ItemLimit)
{
	TArray<AInventoryItem*> OutItems;
	bool HasLimit = ItemLimit >= 0;

	if (HasLimit)
	{
		if (OutItems.Num() >= ItemLimit)return OutItems;
	}

	for (AInventoryItem* LocalItem : GridItems)
	{
		FIntPoint TargetItemSize = LocalItem->ItemTableData.Size;
		FIntPoint TargetItemPos = LocalItem->Position;

		if (IsOverlappingItem(ItemSize, Position, TargetItemSize, TargetItemPos))
		{
			OutItems.Add(LocalItem);
			if (HasLimit)
			{
				if (OutItems.Num() >= ItemLimit)return OutItems;
			}
		}
	}

	return OutItems;
}

bool UInventoryComponent::IsOverlappingItem(FIntPoint ItemSize_A, FIntPoint ItemPos_A, FIntPoint ItemSize_B, FIntPoint ItemPos_B)
{
	FIntPoint A_Min = ItemPos_A;
	FIntPoint A_Max = ItemPos_A + ItemSize_A;

	FIntPoint B_Min = ItemPos_B;
	FIntPoint B_Max = ItemPos_B + ItemSize_B;

	return 	(A_Min.X < B_Max.X && A_Max.X > B_Min.X &&
		A_Min.Y < B_Max.Y && A_Max.Y > B_Min.Y);
}

int32 UInventoryComponent::GetGoldCount()
{
	if (!GoldItemRef)return 0;
	return GoldItemRef->Count;
}

void UInventoryComponent::SetGoldCount(int32 InCount)
{
	if (GoldItemRef)
	{
		GoldItemRef->SetCount(InCount);
	}
	else
	{
		if (InCount > 0)
		{
			if (FInventoryItemTableData* GoldItemRow = ItemsTable->FindRow<FInventoryItemTableData>("Gold", "Cant find gold item row"))
			{
				TSubclassOf<AInventoryItem> GoldClass = UInventorySystemLibrary::InventoryItemTableData_GetItemClass(*GoldItemRow);

				AInventoryItem* GoldRef = GetWorld()->SpawnActorDeferred<AInventoryItem>(GoldClass, FTransform(), nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
				GoldRef->FinishSpawning(FTransform());
				SetGoldItem(GoldRef);
			}
		}
	}
}

void UInventoryComponent::AddGoldCount(int32 InCount)
{
	SetGoldCount(GetGoldCount() + InCount);
}

bool UInventoryComponent::SetGoldItem(AInventoryItem* Item)
{
	if (GoldItemRef == Item)return false;
	if (Item)
	{
		if (!bEnableGold)return false;
		if (Item->ItemRowName != "Gold")return false;
		Item->RemoveFromParent();
	}

	AInventoryItem* PrevItem = GoldItemRef;
	GoldItemRef = Item;
	OnUpdateGoldItem.Broadcast(GoldItemRef, PrevItem);
	return true;
}
bool UInventoryComponent::AddGoldItem(AInventoryItem* Item)
{
	if (!Item)return false;
	if (GoldItemRef)
	{
		if (GoldItemRef->CompleteItem(Item))return true;
	}
	else
	{
		if (SetGoldItem(Item))return true;
	}
	return false;
}

void UInventoryComponent::OnUpdateGoldItem_Event(AInventoryItem* NewItem, AInventoryItem* PrevItem)
{
	if (PrevItem)
	{
		PrevItem->SetupParent(FInventoryItemParentData());
		OnRemoveItem.Broadcast(PrevItem);
	}
	if (NewItem)
	{
		FInventoryItemParentData ParentData;

		ParentData.Type = EInventoryItemParentType::EIIPT_InventoryGold;
		ParentData.Inventory = this;

		NewItem->SetupParent(ParentData);
		OnAddItem.Broadcast(NewItem);
	}
}

void UInventoryComponent::DragItem(AInventoryItem* Item)
{
	if (!Item || IsDragging())return;

	Item->RemoveFromParent();
	SetDragItem(Item);
}

void UInventoryComponent::SetDragItem(AInventoryItem* Item)
{
	if (Item == CurrentDragItem)return;

	if (Item)Item->RemoveFromParent();
	AInventoryItem* PrevItem = CurrentDragItem;
	CurrentDragItem = Item;
	OnUpdateDragItem.Broadcast(CurrentDragItem, PrevItem);
}

bool UInventoryComponent::IsDragging()
{
	return IsValid(CurrentDragItem);
}

void UInventoryComponent::DropItemToGrid(UInventoryComponent* TargetInventory, FIntPoint Position)
{
	if (!IsDragging())return;

	if (TargetInventory->bTraderInventory)
	{
		SellItem(CurrentDragItem);
		return;
	}

	TArray<FIntPoint> ValidCells = TargetInventory->FindGridValidCells();
	FIntPoint ItemSize = CurrentDragItem->ItemTableData.Size;

	if (!TargetInventory->IsFreeGridPositionForItem(ItemSize, Position, ValidCells))return;

	TArray<AInventoryItem*> OverlappedItems = TargetInventory->GetOverlappingItems(ItemSize, Position, 2);
	if (OverlappedItems.Num() <= 1)
	{
		AInventoryItem* SavedDragItem = CurrentDragItem;
		AInventoryItem* OverlappedItem = nullptr;
		if (OverlappedItems.IsValidIndex(0))
		{
			OverlappedItem = OverlappedItems[0];
			OverlappedItem->RemoveFromParent();
		}

		SetDragItem(OverlappedItem);
		TargetInventory->AddItemToGridByPosition_Fast(SavedDragItem, Position);
	}
}

void UInventoryComponent::DropItemToSlot(UInventoryComponent* TargetInventory, EItemSlotType Slot)
{
	if (!IsDragging())return;
	if (!TargetInventory->IsEnableSlotItem(Slot, CurrentDragItem))return;

	AInventoryItem* SavedDragItem = CurrentDragItem;
	AInventoryItem* OverlappedItem = TargetInventory->GetSlotItem(Slot);

	if (OverlappedItem)OverlappedItem->RemoveFromParent();

	SetDragItem(OverlappedItem);
	int32 SlotID = TargetInventory->FindSlotID(Slot);
	TargetInventory->SetSlotItem_Fast(Slot, SlotID, SavedDragItem);
}

void UInventoryComponent::DropItemToFloor(AInventoryItem* Item)
{
	if (!Item)return;

	FTransform LocalDropTransform = GetOwner()->GetActorTransform();

	if (ACharacter* LocalChar = Cast<ACharacter>(GetOwner()))
	{
		float CapsuleHalfHeight = LocalChar->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

		FVector Offset;
		Offset.Z = -CapsuleHalfHeight;

		LocalDropTransform = FTransform(Offset) * LocalDropTransform;
	}

	Item->RemoveFromParent();

	TSubclassOf<APickUp> PickUpClass = UInventorySystemLibrary::InventoryItemTableData_GetPickUpClass(Item->ItemTableData);
	APickUp* PickUpRef = GetWorld()->SpawnActorDeferred<APickUp>(PickUpClass, LocalDropTransform, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	PickUpRef->Item = Item->ItemRowName;
	PickUpRef->Count = Item->Count;
	PickUpRef->InventoryItemRef = Item;

	PickUpRef->FinishSpawning(LocalDropTransform);
}

void UInventoryComponent::OnUpdateDragItem_Event(AInventoryItem* NewItem, AInventoryItem* PrevItem)
{
	if (PrevItem)
	{
		PrevItem->SetupParent(FInventoryItemParentData());
		OnRemoveItem.Broadcast(PrevItem);
	}
	if (NewItem)
	{
		FInventoryItemParentData ParentData;

		ParentData.Type = EInventoryItemParentType::EIIPT_InventoryDrag;
		ParentData.Inventory = this;

		NewItem->SetupParent(ParentData);
		OnAddItem.Broadcast(NewItem);
	}
}

void UInventoryComponent::InspectInventory(UInventoryComponent* InventoryRef)
{
	SetInspectInventory(InventoryRef);
	SetOpen(true);
}

void UInventoryComponent::SetInspectInventory(UInventoryComponent* InventoryRef)
{
	UInventoryComponent* PrevInventory = InspectingInventory;
	InspectingInventory = InventoryRef;
	if (InspectingInventory != PrevInventory)
	{
		OnUpdateInspectInventory.Broadcast(InspectingInventory, PrevInventory);
	}
}

void UInventoryComponent::SetUseRelativeItem(EUseRelativeActionList Action, AInventoryItem* Item)
{
	FUseRelativeData NewData;
	NewData.Action = Action;
	NewData.Item = Item;
	if (UInventorySystemLibrary::UseRelativeData_NotEqual(NewData, UseRelativeData))
	{
		FUseRelativeData PrevData = UseRelativeData;
		UseRelativeData = NewData;
		OnUpdateUseRelativeData.Broadcast(UseRelativeData, PrevData);
	}
}

bool UInventoryComponent::IsRelativeUsing()
{
	return UseRelativeData.Action != EUseRelativeActionList::EURAL_None;
}

void UInventoryComponent::OnUpdateUseRelativeData_Event(FUseRelativeData NewData, FUseRelativeData PrevData)
{
	if (PrevData.Item)
	{
		PrevData.Item->OnEndPlay.RemoveDynamic(this, &UInventoryComponent::UseRelativeItem_OnEndPlay_Event);
	}
	if (NewData.Item)
	{
		NewData.Item->OnEndPlay.AddDynamic(this, &UInventoryComponent::UseRelativeItem_OnEndPlay_Event);
	}
}

void UInventoryComponent::UseRelativeItem_OnEndPlay_Event(AActor* Actor, EEndPlayReason::Type EndPlayReason)
{
	if (EndPlayReason == EEndPlayReason::Destroyed)
	{
		SetUseRelativeItem(EUseRelativeActionList::EURAL_None, nullptr);
	}
}

void UInventoryComponent::UseRelative_Implementation(AInventoryItem* TargetItem)
{
	if (UseRelativeData.Action == EUseRelativeActionList::EURAL_Item)
	{
		UseRelativeData.Item->UseRelative(this, TargetItem);
	}
	if (UseRelativeData.Action == EUseRelativeActionList::EURAL_Forge)
	{
		Forge(TargetItem);
	}
}

bool UInventoryComponent::Forge(AInventoryItem* Item)
{
	if (!Item)return false;

	if (Item->CurrentLevel < Item->ItemTableData.MaxLevel)
	{
		if (IsEnoughMaterialsToForge(Item))
		{
			FItemCountGroup ForgeMaterialGroup = Item->ItemTableData.UpgradeItems[Item->CurrentLevel];

			for (FCraftItemData ItemLocal : ForgeMaterialGroup.Items)
			{
				RemoveItemCount(ItemLocal.Item, ItemLocal.Count);
			}
			Item->AddCurrentLevel(1);
			return true;
		}
	}

	return false;
}

bool UInventoryComponent::IsEnoughMaterialsToForge(AInventoryItem* Item)
{
	if (!Item)return false;

	FItemCountGroup ForgeMaterialGroup = Item->ItemTableData.UpgradeItems[Item->CurrentLevel];

	for (FCraftItemData ItemLocal : ForgeMaterialGroup.Items)
	{
		if (GetItemCount(ItemLocal.Item) < ItemLocal.Count)return false;
	}
	return true;
}

void UInventoryComponent::SetOpen(bool bOpen)
{
	if (IsOpen != bOpen)
	{
		IsOpen = bOpen;
		OnOpenUpdate.Broadcast(IsOpen);
	}
}

bool UInventoryComponent::AddItemRef(AInventoryItem* Item)
{
	if (AddGoldItem(Item))return true;
	if (CompleteItemToSlots(Item))return true;
	if (CompleteItemToGrid(Item))return true;
	if (AddNewItemToSlots(Item))return true;
	if (AddNewItemToGrid(Item))return true;
	return false;
}

void UInventoryComponent::RemoveItemCount(FName Item, int32 Count)
{
	if (Count <= 0)return;

	for (AInventoryItem* LocalItem : GridItems)
	{
		if (LocalItem->ItemRowName == Item)
		{
			int32 CanRemove = FMath::Clamp(Count, 0, LocalItem->Count);
			LocalItem->AddCount(-CanRemove);
			Count -= CanRemove;
			if (Count <= 0)return;
		}
	}

	for (FItemSlot LocalSlot : Slots)
	{
		if (LocalSlot.CurrentItem && LocalSlot.CurrentItem->ItemRowName == Item)
		{
			int32 CanRemove = FMath::Clamp(Count, 0, LocalSlot.CurrentItem->Count);
			LocalSlot.CurrentItem->AddCount(-CanRemove);
			Count -= CanRemove;
			if (Count <= 0)return;
		}
	}

	if (GoldItemRef && GoldItemRef->ItemRowName == Item)
	{
		int32 CanRemove = FMath::Clamp(Count, 0, GoldItemRef->Count);
		GoldItemRef->AddCount(-CanRemove);
		Count -= CanRemove;
		if (Count <= 0)return;
	}
}

void UInventoryComponent::RemoveItem(AInventoryItem* Item)
{
	RemoveItemFromGrid(Item);
	RemoveItemFromSlots(Item);
	if (CurrentDragItem == Item)SetDragItem(nullptr);
	if (GoldItemRef == Item)SetGoldItem(nullptr);
}

void UInventoryComponent::OnAddItem_Event(AInventoryItem* Item)
{
	Item->AttachToActor(GetOwner(), FAttachmentTransformRules::SnapToTargetIncludingScale);
}

void UInventoryComponent::OnRemoveItem_Event(AInventoryItem* Item)
{
	Item->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
}

void UInventoryComponent::Item_OnRemoveFromParent_Event(AInventoryItem* Item)
{
	RemoveItem(Item);
}

void UInventoryComponent::OnOpenUpdate_Event(bool bOpen)
{
	if (!bOpen)
	{
		SetInspectInventory(nullptr);
		SetUseRelativeItem(EUseRelativeActionList::EURAL_None, nullptr);
	}
}

bool UInventoryComponent::HaveItem(FName Item)
{
	return IsValid(GetItem(Item));
}

bool UInventoryComponent::HaveItemRef(AInventoryItem* Item)
{
	if (!Item)return false;
	return Item->ParentData.Inventory == this;
}

void UInventoryComponent::AddItem(FName Item, int32 Count)
{
	if (FInventoryItemTableData* ItemRow = ItemsTable->FindRow<FInventoryItemTableData>(Item, "Cant find item row"))
	{
		TSubclassOf<AInventoryItem> ItemClass = UInventorySystemLibrary::InventoryItemTableData_GetItemClass(*ItemRow);
		AInventoryItem* SpawnedItem = GetWorld()->SpawnActorDeferred<AInventoryItem>(ItemClass, FTransform(), nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		SpawnedItem->ItemRowName = Item;
		SpawnedItem->Count = Count;

		SpawnedItem->FinishSpawning(FTransform());
		AddItemRef(SpawnedItem);
	}
}

void UInventoryComponent::CreateStartItems()
{
	for (FChestItemData ItemData : StartItems)
	{
		if (!UKismetMathLibrary::RandomBoolWithWeight(ItemData.Chance))return;

		if (FInventoryItemTableData* ItemRow = ItemsTable->FindRow<FInventoryItemTableData>(ItemData.Item, "Cant find item row"))
		{
			TSubclassOf<AInventoryItem> ItemClass = UInventorySystemLibrary::InventoryItemTableData_GetItemClass(*ItemRow);
			AInventoryItem* SpawnedItem = GetWorld()->SpawnActorDeferred<AInventoryItem>(ItemClass, FTransform(), nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

			SpawnedItem->ItemRowName = ItemData.Item;
			SpawnedItem->Count = UKismetMathLibrary::RandomIntegerInRange(ItemData.MinCount, ItemData.MaxCount);

			SpawnedItem->FinishSpawning(FTransform());

			if (ItemData.bUseCustomPosition)
			{
				AddItemToGridByPosition_Fast(SpawnedItem, ItemData.CustomPosition);
			}
			else
			{
				AddItemRef(SpawnedItem);
			}
		}
	}
}

AInventoryItem* UInventoryComponent::GetItem(FName Item)
{
	if (GoldItemRef && GoldItemRef->ItemRowName == Item)return GoldItemRef;
	if (CurrentDragItem && CurrentDragItem->ItemRowName == Item)return CurrentDragItem;
	for (FItemSlot LocalSlot : Slots)
	{
		if (LocalSlot.CurrentItem && LocalSlot.CurrentItem->ItemRowName == Item)return LocalSlot.CurrentItem;
	}
	for (AInventoryItem* LocalItem : GridItems)
	{
		if (LocalItem->ItemRowName == Item)return LocalItem;
	}

	return nullptr;
}

void UInventoryComponent::BuyItem(AInventoryItem* Item)
{
	if (!Item)return;

	int32 ItemCost = Item->ItemTableData.Cost;
	if (GetGoldCount() >= ItemCost)
	{
		AInventoryItem* CopiedItem = Item->CopyItem(1);
		if (AddItemRef(CopiedItem))
		{
			AddGoldCount(-ItemCost);
		}
		else
		{
			CopiedItem->Destroy();
		}
	}
}

int32 UInventoryComponent::GetItemCount(FName Item)
{
	int32 OutCount = 0;
	if (GoldItemRef && GoldItemRef->ItemRowName == Item)OutCount += GoldItemRef->Count;
	if (CurrentDragItem && CurrentDragItem->ItemRowName == Item)OutCount += CurrentDragItem->Count;
	for (FItemSlot LocalSlot : Slots)
	{
		if (LocalSlot.CurrentItem && LocalSlot.CurrentItem->ItemRowName == Item)OutCount += LocalSlot.CurrentItem->Count;
	}
	for (AInventoryItem* LocalItem : GridItems)
	{
		if (LocalItem->ItemRowName == Item)OutCount += LocalItem->Count;
	}

	return OutCount;
}

void UInventoryComponent::SellItem(AInventoryItem* Item)
{
	if (!Item)return;

	int32 SellCost = Item->ItemTableData.Cost / 2;
	AddGoldCount(Item->Count * SellCost);
	Item->Destroy();
}
