// Fill out your copyright notice in the Description page of Project Settings.


#include "BuffSystem/BuffInstance.h"
#include "GamePropertySystem/GamePropertyComponent.h"

// Sets default values
ABuffInstance::ABuffInstance()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Properties_Base = CreateDefaultSubobject<UGamePropertyComponent>("Properties_Base");
	Properties_Sum = CreateDefaultSubobject<UGamePropertyComponent>("Properties_Sum");
}

// Called when the game starts or when spawned
void ABuffInstance::BeginPlay()
{
	Super::BeginPlay();
	
	OnUpdateRemaining.AddDynamic(this, &ABuffInstance::OnUpdateRemaining_Event);
	OnUpdateCount.AddDynamic(this, &ABuffInstance::OnUpdateCount_Event);
	UpdateSumProps();

}

// Called every frame
void ABuffInstance::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateRemaining(DeltaTime);
}

void ABuffInstance::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	BuffTableData = FBuffTableData();

	if (BuffsTable)
	{
		if (FBuffTableData* BuffRow = BuffsTable->FindRow<FBuffTableData>(Buff, "Cant find buff row"))
		{
			BuffTableData = *BuffRow;
		}
	}

	Remaining = BuffTableData.Duration;
	Properties_Base->Props = BuffTableData.Props;
}

void ABuffInstance::SetRemaining(float Value)
{
	float PrevValue = Remaining;
	float NewValue = FMath::Clamp(Value, Value, BuffTableData.Duration);

	if (NewValue != PrevValue)
	{
		Remaining = NewValue;
		OnUpdateRemaining.Broadcast(NewValue, PrevValue);
	}
}

void ABuffInstance::AddRemaining(float Value)
{
	SetRemaining(Remaining + Value);
}

void ABuffInstance::OnUpdateRemaining_Event(float NewValue, float PrevValue)
{
	if (NewValue <= 0.0f && NewValue != -1.0f)
	{
		Destroy();
	}
}

void ABuffInstance::UpdateRemaining(float DeltaTime)
{
	if (Remaining >= 0.0f)
	{
		float PrevRemaining = Remaining;
		AddRemaining(DeltaTime * -1.0f);
		UpdateBuff(PrevRemaining - Remaining);
	}
	else
	{
		UpdateBuff(DeltaTime);
	}
}

void ABuffInstance::SetCount(int32 InCount)
{
	int32 PrevCount = Count;
	int32 NewCount = FMath::Clamp(InCount, 0, InCount);

	if (NewCount != PrevCount)
	{
		Count = NewCount;
		OnUpdateCount.Broadcast(NewCount, PrevCount);
		if (Count <= 0)
		{
			Destroy();
		}
	}
}

void ABuffInstance::AddCount(int32 InCount)
{
	SetCount(Count + InCount);
}

void ABuffInstance::OnUpdateCount_Event(int32 NewCount, int32 PrevCount)
{
	UpdateSumProps();
}

void ABuffInstance::UpdateSumProps()
{
	for (FGamePropertyData PropData : Properties_Base->Props)
	{
		float SumValue = PropData.Value;
		if(BuffTableData.bMultiplyPropsByCount)SumValue *= Count;
		
		Properties_Sum->SetPropValue(PropData.Name, SumValue);
	}
}

