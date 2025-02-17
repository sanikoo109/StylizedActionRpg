// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySystem/DesignItemLogicComp.h"
#include "InventorySystem/ItemDesignInterface.h"
#include "Materials/MaterialInstanceDynamic.h"

// Sets default values for this component's properties
UDesignItemLogicComp::UDesignItemLogicComp()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UDesignItemLogicComp::BeginPlay()
{
	Super::BeginPlay();

	if (bInitDynamicMaterials)
	{
		if (!GetOwner()->Implements<UItemDesignInterface>())return;
		UMeshComponent* ItemMesh = IItemDesignInterface::Execute_I_ItemDesign_GetMesh(GetOwner());

		TArray<UMaterialInterface*> MaterialList = ItemMesh->GetMaterials();

		for (int i = 0; i < MaterialList.Num(); ++i)
		{
			UMaterialInstanceDynamic* DynMat = UMaterialInstanceDynamic::Create(MaterialList[i], this);
			ItemMesh->SetMaterial(i, DynMat);
		}
	}
}


// Called every frame
void UDesignItemLogicComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UDesignItemLogicComp::SetItemVisible(bool bVisible)
{
	bItemVisible = bVisible;
	UpdateVisible();
}

void UDesignItemLogicComp::UpdateVisible()
{
	if (!GetOwner()->Implements<UItemDesignInterface>())return;

	IItemDesignInterface::Execute_I_ItemDesign_UpdateVisible(GetOwner(), bItemVisible);
}

void UDesignItemLogicComp::SetParentItem(AInventoryItem* Item)
{
	AInventoryItem* PrevItem = ParentItem;
	ParentItem = Item;

	if (ParentItem != PrevItem)
	{
		OnUpdateParentItem.Broadcast(ParentItem, PrevItem);
	}
}

void UDesignItemLogicComp::SetShowMultipleMesh(bool Value)
{
	bShowMultipleMesh = Value;
}

AActor* UDesignItemLogicComp::GetOwnerActor()
{
	if (!ParentItem)return nullptr;
	return ParentItem->GetOwnerActor();
}

