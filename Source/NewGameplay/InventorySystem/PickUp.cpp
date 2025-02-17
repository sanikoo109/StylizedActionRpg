// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySystem/PickUp.h"
#include "InventorySystem/InventorySystemDeveloperSettings.h"
#include "InventorySystem/InventorySystemLibrary.h"
#include "GameFramework/Character.h"
#include "InventorySystem/InventoryComponent.h"

#define LOCTEXT_NAMESPACE "PickUp"

// Sets default values
APickUp::APickUp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootScene = CreateDefaultSubobject<USceneComponent>("RootScene");
	RootComponent = RootScene;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>("ItemMesh");
	ItemMesh->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void APickUp::BeginPlay()
{
	Super::BeginPlay();
	
	if (!InventoryItemRef)
	{
		InventoryItemRef = SpawnItem();
	}

	InventoryItemRef->AttachToComponent(ItemMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, "Center");
}

void APickUp::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	bUpdate = false;

	if (UDataTable* ItemsTable = Cast<UDataTable>(GetDefault<UInventorySystemDeveloperSettings>()->ItemsTable.TryLoad()))
	{
		if (FInventoryItemTableData* ItemRow = ItemsTable->FindRow<FInventoryItemTableData>(Item, "Cant find item row"))
		{
			UStaticMesh* SelectedMesh = ItemRow->PickUpMesh;
			FTransform SelectedOffset = ItemRow->PickUpOffset;
			if (Count > 1)
			{
				SelectedMesh = ItemRow->PickUpMesh_Multiple;
				SelectedOffset = ItemRow->PickUpOffset_Multiple;
			}
			ItemMesh->SetStaticMesh(SelectedMesh);
			ItemMesh->SetRelativeTransform(SelectedOffset);
		}
	}
}

// Called every frame
void APickUp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APickUp::Interact_Implementation(ACharacter* Char, UPrimitiveComponent* TargetComp)
{
	if (!Char)return;
	if (UInventoryComponent* InventoryComp = Char->FindComponentByClass<UInventoryComponent>())
	{
		if (InventoryComp->AddItemRef(InventoryItemRef))
		{
			Destroy();
		}
	}
}

FInteractInfo APickUp::GetInteractInfo_Implementation(ACharacter* Char, UPrimitiveComponent* TargetComp)
{
	FInteractInfo OutInfo;
	OutInfo.Type = EInteractType::EIT_Default;
	OutInfo.Text = LOCTEXT("PickUpText", "Pick up: {ItemName}{CountText}");

	FNumberFormattingOptions NumFormat;
	NumFormat.UseGrouping = true;

	FText CountText;
	if (Count > 1)
	{
		CountText = FText::AsNumber(Count, &NumFormat);
		CountText = CountText.Format(FText::FromString(" {0}"), FFormatOrderedArguments({ CountText }));
	}

	FFormatNamedArguments Args;
	Args.Add("ItemName", InventoryItemRef->ItemTableData.Name);
	Args.Add("CountText", CountText);

	OutInfo.Text = OutInfo.Text.Format(OutInfo.Text, Args);
	return OutInfo;
}

AInventoryItem* APickUp::SpawnItem_Implementation()
{
	if (UDataTable* ItemsTable = Cast<UDataTable>(GetDefault<UInventorySystemDeveloperSettings>()->ItemsTable.TryLoad()))
	{
		if (FInventoryItemTableData* ItemRow = ItemsTable->FindRow<FInventoryItemTableData>(Item, "Cant find item row"))
		{
			TSubclassOf<AInventoryItem> ItemClass = UInventorySystemLibrary::InventoryItemTableData_GetItemClass(*ItemRow);
			AInventoryItem* CreatedItem = GetWorld()->SpawnActorDeferred<AInventoryItem>(ItemClass, FTransform(), nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
			CreatedItem->ItemRowName = Item;
			CreatedItem->Count = Count;
			CreatedItem->FinishSpawning(FTransform());
			return CreatedItem;
		}
	}
	return nullptr;
}

#undef LOCTEXT_NAMESPACE