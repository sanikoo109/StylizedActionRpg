// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySystem/DIC_ItemSlot.h"
#include "InventorySystem/InventoryComponent.h"
#include "InventorySystem/InventorySystemLibrary.h"
#include "InventorySystem/ItemDesignFunctionLibrary.h"

void UDIC_ItemSlot::BeginPlay()
{
	Super::BeginPlay();

	SavedStartSocket = GetAttachSocketName();
	ParentInventoryComp = GetOwner()->FindComponentByClass<UInventoryComponent>();

	if (ParentInventoryComp)
	{
		ParentInventoryComp->OnUpdateSelectedItem.AddDynamic(this, &UDIC_ItemSlot::OnUpdateSelectedItem_Event);
	}
}

void UDIC_ItemSlot::OnUpdateSelectedItem_Event(EItemSlotType InSlot, AInventoryItem* NewItem, AInventoryItem* PrevItem)
{
	if (Slot != InSlot)return;

	bool bValidItem = false;

	if (NewItem)
	{
		if (bShowMultipleMesh)
		{
			if (NewItem->ItemTableData.bDesignWithMultipleMesh)bValidItem = true;
		}
		else
		{
			bValidItem = true;
		}
	}

	if (bValidItem)
	{
		TSubclassOf<AActor> DesignItemClass = UInventorySystemLibrary::InventoryItemTableData_GetItemDesignClass(NewItem->ItemTableData);
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Owner = NewItem;
		AActor* DesignItemRef = GetWorld()->SpawnActor<AActor>(DesignItemClass, FTransform(), SpawnParams);

		UItemDesignFunctionLibrary::ItemDesign_SetShowMultipleMesh(DesignItemRef, bShowMultipleMesh);
		UItemDesignFunctionLibrary::ItemDesign_SetParentItem(DesignItemRef, NewItem);

		SetItem(DesignItemRef);

		FName SocketL = NewItem->ItemTableData.WeaponAttachSocketL;
		FName SocketR = NewItem->ItemTableData.WeaponAttachSocketR;
		FName SocketMultiL = NewItem->ItemTableData.WeaponAttachSocketL_Multiple;
		FName SocketMultiR = NewItem->ItemTableData.WeaponAttachSocketR_Multiple;

		bool bLeftSlot = UInventorySystemLibrary::ItemSlot_IsLeft(Slot);

		FName AttachSocket = bLeftSlot ? SocketL : SocketR;

		if (bShowMultipleMesh)
		{
			AttachSocket = bLeftSlot ? SocketMultiL : SocketMultiR;
		}

		if (AttachSocket == "")AttachSocket = SavedStartSocket;

		AttachToComponent(GetAttachParent(), FAttachmentTransformRules::SnapToTargetIncludingScale, AttachSocket);
	}
	else
	{
		SetItem(nullptr);
		AttachToComponent(GetAttachParent(), FAttachmentTransformRules::SnapToTargetIncludingScale, SavedStartSocket);
	}
}
