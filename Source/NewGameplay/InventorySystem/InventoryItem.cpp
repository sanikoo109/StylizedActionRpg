// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySystem/InventoryItem.h"
#include "InventorySystem/InventoryComponent.h"
#include "InventorySystem/InventoryItemSlot.h"
#include "InventorySystem/InventorySystemDeveloperSettings.h"
#include "BuffSystem/BuffComponent.h"
#include "GamePropertySystem/GamePropertyComponent.h"
#include "CombatSystem/CombatSystemLibrary.h"

// Sets default values
AInventoryItem::AInventoryItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BuffComponent = CreateDefaultSubobject<UBuffComponent>("BuffComponent");
	PropComponent_ItemBase = CreateDefaultSubobject<UGamePropertyComponent>("PropComponent_ItemBase");
	PropComponent_ItemSum = CreateDefaultSubobject<UGamePropertyComponent>("PropComponent_ItemSum");
	PropComponent_ItemBonus = CreateDefaultSubobject<UGamePropertyComponent>("PropComponent_ItemBonus");
	PropComponent_Character = CreateDefaultSubobject<UGamePropertyComponent>("PropComponent_Character");
}

// Called when the game starts or when spawned
void AInventoryItem::BeginPlay()
{
	Super::BeginPlay();
	
	OnUpdateSocketCount.AddDynamic(this, &AInventoryItem::OnUpdateSocketCount_Event);
	OnUpdateCount.AddDynamic(this, &AInventoryItem::OnUpdateCount_Event);
	OnUpdateSocketItem.AddDynamic(this, &AInventoryItem::OnUpdateSocketItem_Event);
	OnUpdateParentType.AddDynamic(this, &AInventoryItem::OnUpdateParentType_Event);
	OnUpdateCurrentLevel.AddDynamic(this, &AInventoryItem::OnUpdateCurrentLevel_Event);

	BuffComponent->OnAddBuff.AddDynamic(this, &AInventoryItem::OnAddBuff_Event);
	BuffComponent->OnRemoveBuff.AddDynamic(this, &AInventoryItem::OnRemoveBuff_Event);

	PropComponent_ItemSum->RegisterChildProps(PropComponent_ItemBase);
	PropComponent_ItemSum->RegisterChildProps(PropComponent_ItemBonus);
}

void AInventoryItem::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (UDataTable* ItemsTable = Cast<UDataTable>(GetDefault<UInventorySystemDeveloperSettings>()->ItemsTable.TryLoad()))
	{
		if (FInventoryItemTableData* ItemRow = ItemsTable->FindRow<FInventoryItemTableData>(ItemRowName, "Cant find item row"))
		{
			ItemTableData = *ItemRow;
			PropComponent_Character->Props = ItemTableData.Props;
			UpdateDamage();
		}
	}
}

void AInventoryItem::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	if (EndPlayReason == EEndPlayReason::Destroyed)
	{
		RemoveFromParent();
	}
}

// Called every frame
void AInventoryItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

AInventoryItem* AInventoryItem::GetSocketItem(int32 SocketID)
{
	if (!SocketItems.IsValidIndex(SocketID))return nullptr;
	return SocketItems[SocketID];
}

bool AInventoryItem::SetSocketItem(AInventoryItem* Item, int32 Socket)
{
	if (Socket < 0 || Socket > SocketCount)return false;
	if (Item && !IsEnableSocketItem(Item))return false;

	AInventoryItem* PrevItem = GetSocketItem(Socket);
	if (Item == PrevItem)return false;

	SetSocketItem_Fast(Item, Socket, PrevItem);
	return true;
}

void AInventoryItem::SetSocketItem_Fast(AInventoryItem* Item, int32 Socket, AInventoryItem* PrevItem)
{
	if (Item)Item->RemoveFromParent();

	SocketItems.SetNum(Socket + 1, EAllowShrinking::No);
	SocketItems[Socket] = Item;
	OnUpdateSocketItem.Broadcast(Socket, Item, PrevItem);
}

void AInventoryItem::DropItemToSocket(AInventoryItem* Item, int32 SocketID)
{
	if (IsEnableSocketItem(Item) && IsEmptySocket(SocketID))
	{
		AInventoryItem* SplittedItem = Item->Split(1);
		SetSocketItem_Fast(SplittedItem, SocketID, nullptr);
	}
}

void AInventoryItem::SetSocketCount(int32 InCount)
{
	int32 PrevSocketCount = SocketCount;
	SocketCount = FMath::Clamp(InCount, 0, ItemTableData.MaxSockets);

	if (SocketCount != PrevSocketCount)
	{
		OnUpdateSocketCount.Broadcast(SocketCount, PrevSocketCount);
	}
}

void AInventoryItem::AddSocketCount(int32 InCount)
{
	SetSocketCount(SocketCount + InCount);
}

bool AInventoryItem::IsEmptySocket(int32 SocketID)
{
	return !GetSocketItem(SocketID);
}

bool AInventoryItem::IsEnableSocketItem(AInventoryItem* Item)
{
	if (Item)
	{
		return Item->ItemTableData.Type == EInventoryItemType::EIIT_SocketItem;
	}
	return false;
}

FGameDamage AInventoryItem::GetDamage()
{
	FGameDamage OutDamage;
	for (EDamageType DamageType : TEnumRange<EDamageType>())
	{
		if (DamageType == EDamageType::EDT_None)continue;

		FName Prop = UCombatSystemLibrary::DamageType_GetDamageProp(DamageType);
		float PropValue = PropComponent_ItemSum->GetPropValue(Prop);
		if (PropValue > 0.0f)
		{
			FGameTypeDamage TypeDmg;

			TypeDmg.Type = DamageType;
			TypeDmg.Value = PropValue;

			OutDamage.TypeDamageList.Add(TypeDmg);
		}
	}
	return OutDamage;
}

AActor* AInventoryItem::GetOwnerActor()
{
	if (ParentData.Type == EInventoryItemParentType::EIIPT_InventorySlot ||
		ParentData.Type == EInventoryItemParentType::EIIPT_InventoryGrid ||
		ParentData.Type == EInventoryItemParentType::EIIPT_InventoryGold ||
		ParentData.Type == EInventoryItemParentType::EIIPT_InventoryDrag)
	{
		return ParentData.Inventory->GetOwner();
	}
	if (ParentData.Type == EInventoryItemParentType::EIIPT_ItemSocket)
	{
		return ParentData.Item;
	}
	if (ParentData.Type == EInventoryItemParentType::EIIPT_Asset)
	{
		return ParentData.AssetSlot->GetOwner();
	}
	return nullptr;
}

void AInventoryItem::UpdateDamage()
{
	Damage = FGameDamage();

	float LevelScale = CurrentLevel;
	if (ItemTableData.UpgradeCurve)LevelScale = ItemTableData.UpgradeCurve->GetFloatValue(LevelScale);

	for (int i = 0; i < ItemTableData.Damage.TypeDamageList.Num(); ++i)
	{
		FGameTypeDamage BaseDmg = ItemTableData.Damage.TypeDamageList[i];
		FGameTypeDamage MaxDmg = ItemTableData.MaxLevelDamage.TypeDamageList[i];
		FGameTypeDamage OutDamage;

		OutDamage.Type = BaseDmg.Type;
		OutDamage.Value = FMath::RoundToFloat(FMath::Lerp(BaseDmg.Value, MaxDmg.Value, LevelScale));

		Damage.TypeDamageList.Add(OutDamage);
		
		PropComponent_ItemBase->SetPropValue(UCombatSystemLibrary::DamageType_GetDamageProp(OutDamage.Type), OutDamage.Value);
	}
}

void AInventoryItem::SetCount(int32 InCount)
{
	int32 PrevCount = Count;
	Count = FMath::Clamp(InCount, 0, ItemTableData.MaxCount);

	if (PrevCount != Count)
	{
		OnUpdateCount.Broadcast(Count, PrevCount);
	}
}

void AInventoryItem::AddCount(int32 InCount)
{
	SetCount(Count + InCount);
}

void AInventoryItem::SetCurrentLevel(int32 InLevel)
{
	int32 PrevLevel = CurrentLevel;
	CurrentLevel = FMath::Clamp(InLevel, 0, ItemTableData.MaxLevel);

	if (PrevLevel != CurrentLevel)
	{
		OnUpdateCurrentLevel.Broadcast(CurrentLevel, PrevLevel);
	}
}

void AInventoryItem::AddCurrentLevel(int32 InLevel)
{
	SetCurrentLevel(CurrentLevel + InLevel);
}

bool AInventoryItem::CompleteItem(AInventoryItem* Item)
{
	if (Item->ItemRowName == ItemRowName)
	{
		Item->Transite(this, Item->Count);
		if (!IsValid(Item))return true;
	}

	return false;
}

void AInventoryItem::Transite(AInventoryItem* TargetItem, int32 InCount)
{
	if (!TargetItem)return;

	int32 PrevTargetCount = TargetItem->Count;

	TargetItem->AddCount(FMath::Clamp(InCount, 0, Count));
	AddCount(-(TargetItem->Count - PrevTargetCount));
}

AInventoryItem* AInventoryItem::Split(int32 InCount)
{
	int32 TargetCount = FMath::Clamp(InCount, 0, Count);

	if (TargetCount <= 0)return nullptr;
	if (TargetCount >= Count)return this;

	AInventoryItem* NewItem = CopyItem(TargetCount);
	AddCount(-TargetCount);
	return NewItem;
}

UTexture2D* AInventoryItem::GetImage_Implementation()
{
	return ItemTableData.Image;
}

void AInventoryItem::SetupParent(FInventoryItemParentData InData)
{
	FInventoryItemParentData PrevData = ParentData;
	ParentData = InData;

	if (ParentData.Type != PrevData.Type)
	{
		OnUpdateParentType.Broadcast(ParentData.Type, PrevData.Type);
	}
}

bool AInventoryItem::CanDropItem(AInventoryItem* Item)
{
	if (!Item)return false;

	return (Item->ItemRowName == ItemRowName) && (Count < ItemTableData.MaxCount);
}

AInventoryItem* AInventoryItem::CopyItem(int32 InCount)
{
	AInventoryItem* OutItem = GetWorld()->SpawnActorDeferred<AInventoryItem>(GetClass(), FTransform(), nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	OutItem->ItemRowName = ItemRowName;
	OutItem->CurrentLevel = CurrentLevel;
	OutItem->Count = InCount;

	OutItem->FinishSpawning(FTransform());
	return OutItem;
}

bool AInventoryItem::CanReload_Implementation()
{
	return false;
}

void AInventoryItem::OnUpdateSocketCount_Event(int32 NewCount, int32 PrevCount)
{
	for (int i = NewCount; i <= PrevCount; ++i)
	{
		SetSocketItem(nullptr, i);
	}
}

void AInventoryItem::OnUpdateCount_Event(int32 NewCount, int32 PrevCount)
{
	if (NewCount <= 0)Destroy();
}

void AInventoryItem::OnUpdateSocketItem_Event(int32 Socket, AInventoryItem* NewItem, AInventoryItem* PrevItem)
{
	if (PrevItem)
	{
		PrevItem->SetupParent(FInventoryItemParentData());
		PropComponent_ItemBonus->UnregisterChildProps(PrevItem->PropComponent_Character);
	}
	if (NewItem)
	{
		FInventoryItemParentData LocalParentData;

		LocalParentData.Type = EInventoryItemParentType::EIIPT_ItemSocket;
		LocalParentData.Item = this;
		LocalParentData.ItemSocketID = Socket;

		NewItem->SetupParent(LocalParentData);
		PropComponent_ItemBonus->RegisterChildProps(NewItem->PropComponent_Character);
	}
}

void AInventoryItem::OnUpdateCurrentLevel_Event(int32 NewLevel, int32 PrevLevel)
{
	UpdateDamage();
}

void AInventoryItem::OnAddBuff_Event(ABuffInstance* BuffRef)
{
	PropComponent_ItemBonus->RegisterChildProps(BuffRef->Properties_Sum);
}

void AInventoryItem::OnRemoveBuff_Event(ABuffInstance* BuffRef)
{
	PropComponent_ItemBonus->UnregisterChildProps(BuffRef->Properties_Sum);
}

void AInventoryItem::WasteAmmo_Implementation(AInventoryItem* Item)
{
	Item->AddCount(-1);
}

void AInventoryItem::RemoveFromParent()
{
	if (ParentData.Type == EInventoryItemParentType::EIIPT_InventorySlot)
	{
		ParentData.Inventory->SetSlotItem(ParentData.InventorySlot, nullptr);
	}
	if (ParentData.Type == EInventoryItemParentType::EIIPT_InventoryGrid)
	{
		ParentData.Inventory->RemoveItemFromGrid(this);
	}
	if (ParentData.Type == EInventoryItemParentType::EIIPT_InventoryGold)
	{
		ParentData.Inventory->SetGoldItem(nullptr);
	}
	if (ParentData.Type == EInventoryItemParentType::EIIPT_InventoryDrag)
	{
		ParentData.Inventory->SetDragItem(nullptr);
	}
	if (ParentData.Type == EInventoryItemParentType::EIIPT_ItemSocket)
	{
		ParentData.Item->SetSocketItem(nullptr, ParentData.ItemSocketID);
	}
	if (ParentData.Type == EInventoryItemParentType::EIIPT_ItemSocket)
	{
		ParentData.AssetSlot->SetItem(nullptr);
	}
}
