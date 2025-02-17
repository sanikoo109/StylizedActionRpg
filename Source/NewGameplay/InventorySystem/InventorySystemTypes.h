// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AimOffsetBlendSpace.h"
#include "CombatSystem/GameDamageTypes.h"
#include "GamePropertySystem/GamePropertyTypes.h"
#include "InventorySystemTypes.generated.h"

UENUM(BlueprintType)
enum class EItemSlotType : uint8
{

	EIST_None UMETA(DisplayName = "None"),
	EIST_Helmet UMETA(DisplayName = "Helmet"),
	EIST_Body UMETA(DisplayName = "Body"),
	EIST_WeaponR UMETA(DisplayName = "WeaponR"),
	EIST_WeaponL UMETA(DisplayName = "WeaponL"),
	EIST_Gloves UMETA(DisplayName = "Gloves"),
	EIST_Boots UMETA(DisplayName = "Boots"),
	EIST_RingR UMETA(DisplayName = "RingR"),
	EIST_RingL UMETA(DisplayName = "RingL"),
	EIST_Amulet UMETA(DisplayName = "Amulet"),
	EIST_Throwing UMETA(DisplayName = "Throwing"),
	EIST_WeaponR2 UMETA(DisplayName = "WeaponR2"),
	EIST_WeaponL2 UMETA(DisplayName = "WeaponL2")

};

UENUM(BlueprintType)
enum class EInventoryItemParentType : uint8
{

	EIIPT_None UMETA(DisplayName = "None"),
	EIIPT_InventorySlot UMETA(DisplayName = "InventorySlot"),
	EIIPT_InventoryGrid UMETA(DisplayName = "InventoryGrid"),
	EIIPT_InventoryGold UMETA(DisplayName = "InventoryGold"),
	EIIPT_InventoryDrag UMETA(DisplayName = "InventoryDrag"),
	EIIPT_ItemSocket UMETA(DisplayName = "ItemSocket"),
	EIIPT_Asset UMETA(DisplayName = "Asset")

};

UENUM(BlueprintType)
enum class EInventoryItemType : uint8
{

	EIIT_None UMETA(DisplayName = "None"),
	EIIT_Weapon UMETA(DisplayName = "Weapon"),
	EIIT_Helmet UMETA(DisplayName = "Helmet"),
	EIIT_Body UMETA(DisplayName = "Body"),
	EIIT_Gloves UMETA(DisplayName = "Gloves"),
	EIIT_Boots UMETA(DisplayName = "Boots"),
	EIIT_Ring UMETA(DisplayName = "Ring"),
	EIIT_Amulet UMETA(DisplayName = "Amulet"),
	EIIT_Throwing UMETA(DisplayName = "Throwing"),
	EIIT_Gold UMETA(DisplayName = "Gold"),
	EIIT_Consumable UMETA(DisplayName = "Consumable"),
	EIIT_Key UMETA(DisplayName = "Key"),
	EIIT_SpellBook UMETA(DisplayName = "SpellBook"),
	EIIT_SocketItem UMETA(DisplayName = "SocketItem"),
	EIIT_Recipe UMETA(DisplayName = "Recipe"),
	EIIT_Material UMETA(DisplayName = "Material")

};

UENUM(BlueprintType)
enum class EInventoryItemRarity : uint8
{

	EIIR_None UMETA(DisplayName = "None"),
	EIIR_Default UMETA(DisplayName = "Default"),
	EIIR_Unique UMETA(DisplayName = "Unique")

};

UENUM(BlueprintType)
enum class EUseRelativeActionList : uint8
{

	EURAL_None UMETA(DisplayName = "None"),
	EURAL_Item UMETA(DisplayName = "Item"),
	EURAL_Forge UMETA(DisplayName = "Forge")

};

USTRUCT(BlueprintType)
struct FItemSlot
{

	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Item Slot")
	EItemSlotType Type;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Item Slot")
	class AInventoryItem* CurrentItem;

};

USTRUCT(BlueprintType)
struct FItemSlotData : public FTableRowBase
{

	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Item Slot")
	FText Name;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Item Slot")
	FIntPoint Size = FIntPoint(1, 1);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Item Slot")
	UTexture2D* BackgroundImage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Item Slot")
	EInventoryItemType EnabledType;

};

USTRUCT(BlueprintType)
struct FInventoryItemParentData
{

	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Item Slot")
	EInventoryItemParentType Type;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Item Slot")
	class UInventoryComponent* Inventory;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Item Slot")
	EItemSlotType InventorySlot;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Item Slot")
	class AInventoryItem* Item;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Item Slot")
	int32 ItemSocketID;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Item Slot")
	class UInventoryItemSlot* AssetSlot;

};

USTRUCT(BlueprintType)
struct FChestItemData
{

	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Chest Item Data")
	FName Item;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Chest Item Data")
	int32 MinCount = 1;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Chest Item Data")
	int32 MaxCount = 1;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Chest Item Data")
	bool bUseCustomPosition;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Chest Item Data")
	FIntPoint CustomPosition;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Chest Item Data")
	float Chance = 1.0f;

};

USTRUCT(BlueprintType)
struct FCraftItemData
{

	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Craft Item Data")
	FName Item;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Craft Item Data")
	int32 Count;

};

USTRUCT(BlueprintType)
struct FItemCountGroup
{

	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Item Count Group")
	TArray<FCraftItemData> Items;

};

USTRUCT(BlueprintType)
struct FUseRelativeData
{

	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Use Relative Data")
	EUseRelativeActionList Action;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Use Relative Data")
	class AInventoryItem* Item;

};

USTRUCT(BlueprintType)
struct FArmsBlendSettings
{

	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Arms Blend Settings")
	float ClavicleLocalSpaceAlpha;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Arms Blend Settings")
	float SpineMeshSpaceAlpha;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Arms Blend Settings")
	float SpineLocalSpaceAlpha;

};

USTRUCT(BlueprintType)
struct FItemBlockingAnimData
{

	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Item Blocking Anim Data")
	UAimOffsetBlendSpace* Aim;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Item Blocking Anim Data")
	UAnimSequence* Sequence;

};

USTRUCT(BlueprintType)
struct FInventoryItemTableData : public FTableRowBase
{

	GENERATED_BODY()

	FInventoryItemTableData()
	{
		SpellCastMove.Add(ESpellCastAnimType::ESCAT_Hand, "CastHand");
		SpellCastMove.Add(ESpellCastAnimType::ESCAT_Hand, "CastFoot");

		ArmsBlendSettings.ClavicleLocalSpaceAlpha = 1.0f;
	}

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory Item")
	EInventoryItemType Type;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory Item")
	FText Name;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory Item")
	FText Description;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory Item")
	UTexture2D* Image;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory Item")
	int32 MaxCount = 1;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory Item")
	FIntPoint Size = FIntPoint(1, 1);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory Item")
	EInventoryItemRarity Rarity = EInventoryItemRarity::EIIR_Default;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory Item")
	FGameDamage Damage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory Item")
	FGameDamage MaxLevelDamage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory Item")
	UCurveFloat* UpgradeCurve;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory Item")
	TArray<FGamePropertyData> Props;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory Item")
	UStaticMesh* PickUpMesh;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory Item")
	UStaticMesh* PickUpMesh_Multiple;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory Item")
	USkeletalMesh* DesignMesh;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory Item")
	FName MoveSet;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory Item")
	TSubclassOf<class AInventoryItem> Class;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory Item")
	TSubclassOf<AActor> DesignClass;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory Item")
	TSubclassOf<class APickUp> PickUpClass;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory Item")
	FName WeaponAttachSocketL;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory Item")
	FName WeaponAttachSocketR;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory Item")
	bool bEnableLeftArm = true;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory Item")
	bool bEnableRightArm = true;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory Item")
	FName UseBuff;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory Item")
	FItemBlockingAnimData BlockingAnimData_R;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory Item")
	FItemBlockingAnimData BlockingAnimData_L;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory Item")
	FName Spell;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory Item")
	TMap<ESpellCastAnimType, FName> SpellCastMove;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory Item")
	UAnimSequence* GripAnim;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory Item")
	FTransform PickUpOffset;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory Item")
	int32 Cost;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory Item")
	int32 MaxSockets;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory Item")
	TArray<FItemCountGroup> UpgradeItems;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory Item")
	int32 MaxLevel;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory Item")
	TSubclassOf<UAnimInstance> AnimInstanceClass;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory Item")
	FName Ammo;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory Item")
	FTransform PickUpOffset_Multiple;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory Item")
	FName WeaponAttachSocketL_Multiple;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory Item")
	FName WeaponAttachSocketR_Multiple;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory Item")
	TSubclassOf<class AProj_Sphere> ProjectileClass;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory Item")
	bool bDesignWithMultipleMesh;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory Item")
	UCurveFloat* AimCurve;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory Item")
	bool bTwoHanded;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory Item")
	bool bAimByDefault;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory Item")
	FArmsBlendSettings ArmsBlendSettings;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory Item")
	UAnimMontage* ReloadMontage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory Item")
	float ReloadRate = 1.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory Item")
	UAnimMontage* ReloadItemMontage;

};