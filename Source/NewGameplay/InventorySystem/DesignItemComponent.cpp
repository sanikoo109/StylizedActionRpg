// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySystem/DesignItemComponent.h"
#include "InventorySystem/ItemDesign_Skeletal.h"
#include "InventorySystem/ItemDesignFunctionLibrary.h"
#include "InventorySystem/ItemDesignInterface.h"

// Sets default values for this component's properties
UDesignItemComponent::UDesignItemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UDesignItemComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UDesignItemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UDesignItemComponent::SetItem(AActor* Item)
{
	if (CurrentItem == Item)return;

	AActor* PrevItem = CurrentItem;
	CurrentItem = Item;

	OnUpdateItem_Event(CurrentItem, PrevItem);
	OnUpdateItem.Broadcast(CurrentItem, PrevItem);
}

void UDesignItemComponent::SetItemVisible(bool IsVisible)
{
	bItemVisible = IsVisible;
	if (CurrentItem)
	{
		UItemDesignFunctionLibrary::ItemDesign_SetVisible(CurrentItem, IsVisible);
	}
}

UMeshComponent* UDesignItemComponent::GetItemMesh()
{
	if (CurrentItem)
	{
		if (CurrentItem->Implements<UItemDesignInterface>())
		{
			return IItemDesignInterface::Execute_I_ItemDesign_GetMesh(CurrentItem);
		}
	}
	return nullptr;
}

void UDesignItemComponent::OnUpdateItem_Event(AActor* NewItem, AActor* PrevItem)
{
	if (PrevItem)
	{
		PrevItem->Destroy();
	}
	if (NewItem)
	{
		NewItem->AttachToComponent(this, FAttachmentTransformRules::SnapToTargetIncludingScale);

		if (bAutoLeadPoseParent)
		{
			if (USkeletalMeshComponent* CharMesh = Cast<USkeletalMeshComponent>(GetAttachParent()))
			{
				if (AItemDesign_Skeletal* DesignSkeletalActor = Cast<AItemDesign_Skeletal>(NewItem))
				{
					DesignSkeletalActor->ItemMesh->SetLeaderPoseComponent(CharMesh);
				}
			}
		}

		UItemDesignFunctionLibrary::ItemDesign_SetVisible(NewItem, bItemVisible);
	}
}
