// Fill out your copyright notice in the Description page of Project Settings.


#include "AttributeSystem/AttributeComponent.h"

// Sets default values for this component's properties
UAttributeComponent::UAttributeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UAttributeComponent::BeginPlay()
{
	Super::BeginPlay();

	OnUpdateCurrentValue.AddDynamic(this, &UAttributeComponent::OnUpdateCurrentValue_Event);
}


// Called every frame
void UAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UAttributeComponent::AddCurrentValue(EAttributeList Attribute, float InValue)
{
	SetCurrentValue(Attribute, GetCurrentValue(Attribute) + InValue);
}

void UAttributeComponent::SetCurrentValue(EAttributeList Attribute, float InValue)
{
	int32 ID = FindAttributeID(Attribute);
	if (ID >= 0)
	{
		float PrevValue = Attributes[ID].CurrentValue;
		float NewValue = FMath::Clamp(InValue, 0.0f, Attributes[ID].MaxValue);
		
		if (NewValue != PrevValue)
		{
			Attributes[ID].CurrentValue = NewValue;
			OnUpdateCurrentValue.Broadcast(Attribute, NewValue, PrevValue);
		}
	}
}

float UAttributeComponent::GetCurrentValue(EAttributeList Attribute)
{
	return GetAttributeData(Attribute).CurrentValue;
}

FAttributeData UAttributeComponent::GetAttributeData(EAttributeList Attribute)
{
	int32 ID = FindAttributeID(Attribute);
	if (ID >= 0)
	{
		return Attributes[ID];
	}
	return FAttributeData();
}

int32 UAttributeComponent::FindAttributeID(EAttributeList Attribute)
{
	for (int i = 0; i < Attributes.Num(); ++i)
	{
		if (Attributes[i].Attribute == Attribute)return i;
	}
	return -1;
}

float UAttributeComponent::GetMaxValue(EAttributeList Attribute)
{
	return GetAttributeData(Attribute).MaxValue;
}

void UAttributeComponent::SetMaxValue(EAttributeList Attribute, float InValue)
{
	int32 ID = FindAttributeID(Attribute);
	if (ID >= 0)
	{
		float PrevValue = Attributes[ID].MaxValue;
		float SavedPercent = Attributes[ID].CurrentValue / Attributes[ID].MaxValue;
		float NewValue = FMath::Clamp(InValue, 0.0f, InValue);

		if (NewValue != PrevValue)
		{
			Attributes[ID].MaxValue = NewValue;
			SetCurrentValue(Attribute, NewValue * SavedPercent);
			OnUpdateMaxValue.Broadcast(Attribute, NewValue, PrevValue);
		}
	}
}

void UAttributeComponent::AddMaxValue(EAttributeList Attribute, float InValue)
{
	SetMaxValue(Attribute, GetMaxValue(Attribute) + InValue);
}

void UAttributeComponent::OnUpdateCurrentValue_Event(EAttributeList Attribute, float NewValue, float PrevValue)
{
	if (NewValue <= 0.0f)
	{
		OnEmpty.Broadcast(Attribute);
	}
	if (NewValue >= GetMaxValue(Attribute))
	{
		OnFull.Broadcast(Attribute);
	}
}

