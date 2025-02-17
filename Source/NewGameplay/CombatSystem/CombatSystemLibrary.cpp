// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatSystem/CombatSystemLibrary.h"

float UCombatSystemLibrary::GetDamageOfType(FGameDamage Data, EDamageType Type)
{
    for (FGameTypeDamage TypeDamage : Data.TypeDamageList)
    {
        if (TypeDamage.Type == Type)return TypeDamage.Value;
    }
    return 0.0f;
}

float UCombatSystemLibrary::GetDamageSum(FGameDamage Data)
{
    return Data.GetSum();
}

FGameDamage UCombatSystemLibrary::ScaleDamage(FGameDamage Damage, float Scale)
{
    for (int i = 0; i < Damage.TypeDamageList.Num(); ++i)
    {
        Damage.TypeDamageList[i].Value *= Scale;
    }
    return Damage;
}

FName UCombatSystemLibrary::DamageType_GetDamageProp(EDamageType DamageType)
{
    if (DamageType == EDamageType::EDT_Physical)return "DamagePhysical";
    if (DamageType == EDamageType::EDT_Cold)return "DamageCold";
    if (DamageType == EDamageType::EDT_Poison)return "DamagePoison";
    if (DamageType == EDamageType::EDT_Fire)return "DamageFire";
    if (DamageType == EDamageType::EDT_Electric)return "DamageElectric";
    if (DamageType == EDamageType::EDT_Magical)return "DamageMagical";
    return FName();
}

FName UCombatSystemLibrary::DamageType_GetDefenseProp(EDamageType DamageType)
{
    if (DamageType == EDamageType::EDT_Physical)return "DefensePhysical";
    if (DamageType == EDamageType::EDT_Cold)return "DefenseCold";
    if (DamageType == EDamageType::EDT_Poison)return "DefensePoison";
    if (DamageType == EDamageType::EDT_Fire)return "DefenseFire";
    if (DamageType == EDamageType::EDT_Electric)return "DefenseElectric";
    if (DamageType == EDamageType::EDT_Magical)return "DefenseMagical";
    return FName();
}
