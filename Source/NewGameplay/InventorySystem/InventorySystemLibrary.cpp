// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySystem/InventorySystemLibrary.h"
#include "InventorySystem/InventorySystemDeveloperSettings.h"

bool UInventorySystemLibrary::ItemSlot_IsSecondary(EItemSlotType Slot)
{
    if (Slot == EItemSlotType::EIST_WeaponR2)return true;
    if (Slot == EItemSlotType::EIST_WeaponL2)return true;
    return false;
}

bool UInventorySystemLibrary::ItemSlot_IsWeapon(EItemSlotType Slot)
{
    if (Slot == EItemSlotType::EIST_WeaponR)return true;
    if (Slot == EItemSlotType::EIST_WeaponL)return true;

    if (Slot == EItemSlotType::EIST_WeaponR2)return true;
    if (Slot == EItemSlotType::EIST_WeaponL2)return true;
    return false;
}

bool UInventorySystemLibrary::ItemSlot_IsLeft(EItemSlotType Slot)
{
    if (Slot == EItemSlotType::EIST_RingL)return true;
    if (Slot == EItemSlotType::EIST_WeaponL)return true;
    if (Slot == EItemSlotType::EIST_WeaponL2)return true;
    return false;
}

EItemSlotType UInventorySystemLibrary::GetWeaponSlot(bool bLeft, bool bSecondary)
{
    if (!bSecondary)
    {
        if (!bLeft)return EItemSlotType::EIST_WeaponR;
        if (bLeft)return EItemSlotType::EIST_WeaponL;
    }
    if (bSecondary)
    {
        if (!bLeft)return EItemSlotType::EIST_WeaponR2;
        if (bLeft)return EItemSlotType::EIST_WeaponL2;
    }
    return EItemSlotType();
}

EItemSlotType UInventorySystemLibrary::ItemSlot_GetPrimary(EItemSlotType Slot)
{
    if (Slot == EItemSlotType::EIST_WeaponR2)return EItemSlotType::EIST_WeaponR;
    if (Slot == EItemSlotType::EIST_WeaponL2)return EItemSlotType::EIST_WeaponL;
    return Slot;
}

TSubclassOf<class AInventoryItem> UInventorySystemLibrary::InventoryItemTableData_GetItemClass(FInventoryItemTableData TableData)
{
    TSubclassOf<AInventoryItem> OutClass = GetDefault<UInventorySystemDeveloperSettings>()->DefaultItemClass;
    if (TableData.Class != NULL)OutClass = TableData.Class;
    return OutClass;
}

TSubclassOf<AActor> UInventorySystemLibrary::InventoryItemTableData_GetItemDesignClass(FInventoryItemTableData TableData)
{
    TSubclassOf<AActor> OutClass = GetDefault<UInventorySystemDeveloperSettings>()->DefaultItemDesignClass;
    if (TableData.DesignClass != NULL)OutClass = TableData.DesignClass;
    return OutClass;
}

TSubclassOf<class APickUp> UInventorySystemLibrary::InventoryItemTableData_GetPickUpClass(FInventoryItemTableData TableData)
{
    TSubclassOf<APickUp> OutClass = GetDefault<UInventorySystemDeveloperSettings>()->DefaultPickUpClass;
    if (TableData.PickUpClass != NULL)OutClass = TableData.PickUpClass;
    return OutClass;
}

bool UInventorySystemLibrary::UseRelativeData_Equal(FUseRelativeData A, FUseRelativeData B)
{
    return (
        A.Action == B.Action &&
        A.Item == B.Item);
}

bool UInventorySystemLibrary::UseRelativeData_NotEqual(FUseRelativeData A, FUseRelativeData B)
{
    return !UInventorySystemLibrary::UseRelativeData_Equal(A, B);
}

EItemSlotType UInventorySystemLibrary::ItemSlot_GetOpposite(EItemSlotType Slot)
{
    if (Slot == EItemSlotType::EIST_RingL)return EItemSlotType::EIST_RingR;
    if (Slot == EItemSlotType::EIST_RingR)return EItemSlotType::EIST_RingL;

    if (Slot == EItemSlotType::EIST_WeaponR)return EItemSlotType::EIST_WeaponL;
    if (Slot == EItemSlotType::EIST_WeaponL)return EItemSlotType::EIST_WeaponR;

    if (Slot == EItemSlotType::EIST_WeaponR2)return EItemSlotType::EIST_WeaponL2;
    if (Slot == EItemSlotType::EIST_WeaponL2)return EItemSlotType::EIST_WeaponR2;

    return EItemSlotType();
}
