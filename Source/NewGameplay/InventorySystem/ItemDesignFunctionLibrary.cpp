// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySystem/ItemDesignFunctionLibrary.h"
#include "InventorySystem/ItemDesignInterface.h"
#include "InventorySystem/DesignItemLogicComp.h"

void UItemDesignFunctionLibrary::ItemDesign_SetVisible(AActor* Target, bool bVisible)
{
	if (!Target || !Target->Implements<UItemDesignInterface>())return;

	if (UDesignItemLogicComp* DesignItemLogicComp = IItemDesignInterface::Execute_I_ItemDesign_GetLogicComp(Target))
	{
		DesignItemLogicComp->SetItemVisible(bVisible);
	}
}

void UItemDesignFunctionLibrary::ItemDesign_UpdateVisible(AActor* Target)
{
	if (!Target || !Target->Implements<UItemDesignInterface>())return;

	if (UDesignItemLogicComp* DesignItemLogicComp = IItemDesignInterface::Execute_I_ItemDesign_GetLogicComp(Target))
	{
		DesignItemLogicComp->UpdateVisible();
	}
}

AInventoryItem* UItemDesignFunctionLibrary::ItemDesign_GetParentItem(AActor* Target)
{
	if (!Target || !Target->Implements<UItemDesignInterface>())return nullptr;

	if (UDesignItemLogicComp* DesignItemLogicComp = IItemDesignInterface::Execute_I_ItemDesign_GetLogicComp(Target))
	{
		return DesignItemLogicComp->ParentItem;
	}
	return nullptr;
}

void UItemDesignFunctionLibrary::ItemDesign_SetParentItem(AActor* Target, AInventoryItem* Item)
{
	if (!Target || !Target->Implements<UItemDesignInterface>())return;

	if (UDesignItemLogicComp* DesignItemLogicComp = IItemDesignInterface::Execute_I_ItemDesign_GetLogicComp(Target))
	{
		DesignItemLogicComp->SetParentItem(Item);
	}
}

void UItemDesignFunctionLibrary::ItemDesign_SetShowMultipleMesh(AActor* Target, bool Value)
{
	if (!Target || !Target->Implements<UItemDesignInterface>())return;

	if (UDesignItemLogicComp* DesignItemLogicComp = IItemDesignInterface::Execute_I_ItemDesign_GetLogicComp(Target))
	{
		DesignItemLogicComp->SetShowMultipleMesh(Value);
	}
}
