// Fill out your copyright notice in the Description page of Project Settings.


#include "GamePropertySystem/GamePropertyComponent.h"

// Sets default values for this component's properties
UGamePropertyComponent::UGamePropertyComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGamePropertyComponent::BeginPlay()
{
	Super::BeginPlay();

	OnRegisterChildProps.AddDynamic(this, &UGamePropertyComponent::OnRegisterChildProps_Event);
	OnUnregisterChildProps.AddDynamic(this, &UGamePropertyComponent::OnUnregisterChildProps_Event);
	OnUpdatePropValue.AddDynamic(this, &UGamePropertyComponent::OnUpdatePropValue_Event);
}


// Called every frame
void UGamePropertyComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UGamePropertyComponent::SetPropValue(FName Prop, float Value)
{
	int32 ID = FindPropID(Prop);
	if (ID >= 0)
	{
		float PrevValue = Props[ID].Value;
		if (Value != PrevValue)
		{
			Props[ID].Value = Value;
			OnUpdatePropValue.Broadcast(Prop, Value, PrevValue);
			if (Value == 0.0f)
			{
				Props.RemoveAt(ID);
			}
		}
	}
	else
	{
		if(Value != 0.0f)
		{
			FGamePropertyData PropData;
			PropData.Value = Value;
			PropData.Name = Prop;
			Props.Add(PropData);
			OnUpdatePropValue.Broadcast(Prop, Value, 0.0f);
		}
	}
}

void UGamePropertyComponent::AddPropValue(FName Prop, float Value)
{
	SetPropValue(Prop, GetPropValue(Prop) + Value);
}

float UGamePropertyComponent::GetPropValue(FName Prop)
{
	return GetPropData(Prop).Value;
}

bool UGamePropertyComponent::IsPropActive(FName Prop)
{
	return GetPropValue(Prop) != 0.0f;
}

bool UGamePropertyComponent::IsPropPositive(FName Prop)
{
	return GetPropValue(Prop) > 0.0f;
}

bool UGamePropertyComponent::IsPropNegative(FName Prop)
{
	return GetPropValue(Prop) < 0.0f;
}

FGamePropertyData UGamePropertyComponent::GetPropData(FName Prop)
{
	int32 ID = FindPropID(Prop);
	if (ID >= 0)
	{
		return Props[ID];
	}
	return FGamePropertyData();
}

int32 UGamePropertyComponent::FindPropID(FName Prop)
{
	for (int i = 0; i < Props.Num(); ++i)
	{
		if (Props[i].Name == Prop)return i;
	}
	return -1;
}

void UGamePropertyComponent::RegisterChildProps(UGamePropertyComponent* PropComp)
{
	if (!PropComp || ChildrenProps.Contains(PropComp))return;

	ChildrenProps.Add(PropComp);
	OnRegisterChildProps.Broadcast(PropComp);
}

void UGamePropertyComponent::UnregisterChildProps(UGamePropertyComponent* PropComp)
{
	if (!PropComp || !ChildrenProps.Contains(PropComp))return;

	ChildrenProps.Remove(PropComp);
	OnUnregisterChildProps.Broadcast(PropComp);
}

void UGamePropertyComponent::UpdateProp(FName Prop)
{
	float LocalSum = 0.0f;
	for (UGamePropertyComponent* PropComp : ChildrenProps)
	{
		LocalSum += PropComp->GetPropValue(Prop);
	}
	SetPropValue(Prop, LocalSum);
}

void UGamePropertyComponent::OnRegisterChildProps_Event(UGamePropertyComponent* PropComp)
{
	PropComp->OnUpdatePropValue.AddDynamic(this, &UGamePropertyComponent::ChildProps_OnUpdatePropValue_Event);
	for (FGamePropertyData PropData : PropComp->Props)
	{
		UpdateProp(PropData.Name);
	}
}

void UGamePropertyComponent::OnUnregisterChildProps_Event(UGamePropertyComponent* PropComp)
{
	PropComp->OnUpdatePropValue.RemoveDynamic(this, &UGamePropertyComponent::ChildProps_OnUpdatePropValue_Event);
	for (FGamePropertyData PropData : PropComp->Props)
	{
		UpdateProp(PropData.Name);
	}
}

void UGamePropertyComponent::OnUpdatePropValue_Event(FName Prop, float NewValue, float PrevValue)
{
	if ((NewValue == 0.0f) != (PrevValue == 0.0f))
	{
		OnUpdatePropActive.Broadcast(Prop, (NewValue != 0.0f));
	}
	if ((NewValue > 0.0f) != (PrevValue > 0.0f))
	{
		OnUpdatePropPositive.Broadcast(Prop, (NewValue > 0.0f));
	}
}

void UGamePropertyComponent::ChildProps_OnUpdatePropValue_Event(FName Prop, float NewValue, float PrevValue)
{
	UpdateProp(Prop);
}

