// Fill out your copyright notice in the Description page of Project Settings.


#include "StatSystem/StatComponent.h"

// Sets default values for this component's properties
UStatComponent::UStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	FStatData Stat_StrengthData;
	FStatData Stat_DexterityData;
	FStatData Stat_IntelligenceData;

	Stat_StrengthData.Stat = EStatList::ESL_Strength;
	Stat_DexterityData.Stat = EStatList::ESL_Dexterity;
	Stat_IntelligenceData.Stat = EStatList::ESL_Intelligence;

	Stats.Add(Stat_StrengthData);
	Stats.Add(Stat_DexterityData);
	Stats.Add(Stat_IntelligenceData);
}


// Called when the game starts
void UStatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UStatComponent::SetStatValue(EStatList Stat, int32 Value)
{
	int32 ID = FindStatID(Stat);
	if (ID >= 0)
	{
		int32 PrevValue = Stats[ID].Value;
		int32 NewValue = FMath::Clamp(Value, 0, Value);

		if (PrevValue != NewValue)
		{
			Stats[ID].Value = NewValue;
			OnUpdateStatValue.Broadcast(Stat, NewValue, PrevValue);
		}
	}
}

void UStatComponent::AddStatValue(EStatList Stat, int32 Value)
{
	SetStatValue(Stat, GetStatValue(Stat) + Value);
}

int32 UStatComponent::GetStatValue(EStatList Stat)
{
	int32 ID = FindStatID(Stat);
	if (ID >= 0)
	{
		return Stats[ID].Value;
	}
	return 0;
}

int32 UStatComponent::FindStatID(EStatList Stat)
{
	for (int i = 0; i < Stats.Num(); ++i)
	{
		if (Stats[i].Stat == Stat)return i;
	}
	return -1;
}

void UStatComponent::SetCurrentPoints(int32 Value)
{
	CurrentPoints = FMath::Clamp(Value, 0, Value);
}

void UStatComponent::AddCurrentPoints(int32 Value)
{
	SetCurrentPoints(CurrentPoints + Value);
}

void UStatComponent::InvestPoint(EStatList InStat)
{
	if (CurrentPoints > 0)
	{
		AddStatValue(InStat, 1);
		AddCurrentPoints(-1);
		OnInvestPoint.Broadcast(InStat);
	}
}
