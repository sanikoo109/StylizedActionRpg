// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySystem/InventoryItemSlot.h"

// Sets default values for this component's properties
UInventoryItemSlot::UInventoryItemSlot()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UInventoryItemSlot::BeginPlay()
{
	Super::BeginPlay();

	OnUpdateItem.AddDynamic(this, &UInventoryItemSlot::OnUpdateItem_Event);
}


// Called every frame
void UInventoryItemSlot::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UInventoryItemSlot::SetItem(AInventoryItem* ItemRef)
{
	if (ItemRef != CurrentItem)
	{
		AInventoryItem* PrevItem = CurrentItem;
		if (ItemRef)ItemRef->RemoveFromParent();
		CurrentItem = ItemRef;
		OnUpdateItem.Broadcast(CurrentItem, PrevItem);
	}
}

bool UInventoryItemSlot::IsEmpty()
{
	return !IsValid(CurrentItem);
}

void UInventoryItemSlot::OnUpdateItem_Event(AInventoryItem* NewItem, AInventoryItem* PrevItem)
{
	if (PrevItem)
	{
		PrevItem->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		PrevItem->SetupParent(FInventoryItemParentData());
	}
	if (NewItem)
	{
		FInventoryItemParentData ParentData;

		ParentData.Type = EInventoryItemParentType::EIIPT_Asset;
		ParentData.AssetSlot = this;

		NewItem->SetupParent(ParentData);
		NewItem->AttachToComponent(this, FAttachmentTransformRules::SnapToTargetIncludingScale);
	}
}

