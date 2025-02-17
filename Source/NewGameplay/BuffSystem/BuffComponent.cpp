// Fill out your copyright notice in the Description page of Project Settings.


#include "BuffSystem/BuffComponent.h"

// Sets default values for this component's properties
UBuffComponent::UBuffComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UBuffComponent::BeginPlay()
{
	Super::BeginPlay();

	OnAddBuff.AddDynamic(this, &UBuffComponent::OnAddBuff_Event);
	OnRemoveBuff.AddDynamic(this, &UBuffComponent::OnRemoveBuff_Event);

}


// Called every frame
void UBuffComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UBuffComponent::PlayBuff(FName Buff)
{
	if (FBuffTableData* BuffRow = BuffsTable->FindRow<FBuffTableData>(Buff, "Cant find buff row"))
	{
		if (BuffRow->Group != "")
		{
			RemoveBuffsInGroup(BuffRow->Group);
		}
		if (ABuffInstance* BuffRef = FindBuff(Buff))
		{
			BuffRef->SetRemaining(BuffRow->Duration);
			BuffRef->SetCount(BuffRow->InitCount);
		}
		else
		{
			TSubclassOf<ABuffInstance> BuffClass = BaseBuffInstanceClass;
			if (BuffRow->InstanceClass != NULL)BuffClass = BuffRow->InstanceClass;
			ABuffInstance* SpawnedBuff = GetWorld()->SpawnActorDeferred<ABuffInstance>(BuffClass, FTransform(), GetOwner(), nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
			SpawnedBuff->ParentBuffComponent = this;
			SpawnedBuff->Buff = Buff;
			SpawnedBuff->Count = BuffRow->InitCount;
			SpawnedBuff->FinishSpawning(FTransform());
			AddBuff(SpawnedBuff);
		}
	}
}

ABuffInstance* UBuffComponent::FindBuff(FName Buff)
{
	for (ABuffInstance* BuffRef : Buffs)
	{
		if (BuffRef->Buff == Buff)return BuffRef;
	}
	return nullptr;
}

bool UBuffComponent::HaveBuff(FName Buff)
{
	if (FindBuff(Buff))return true;
	return false;
}

void UBuffComponent::OnAddBuff_Event(ABuffInstance* BuffRef)
{
	BuffRef->OnEndPlay.AddDynamic(this, &UBuffComponent::Buff_OnEndPlay_Event);
	BuffRef->AttachToActor(GetOwner(), FAttachmentTransformRules::SnapToTargetIncludingScale);
}

void UBuffComponent::OnRemoveBuff_Event(ABuffInstance* BuffRef)
{
	BuffRef->OnEndPlay.RemoveDynamic(this, &UBuffComponent::Buff_OnEndPlay_Event);
	BuffRef->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
}

void UBuffComponent::Buff_OnEndPlay_Event(AActor* Actor, EEndPlayReason::Type EndPlayReason)
{
	if (ABuffInstance* BuffRef = Cast<ABuffInstance>(Actor))
	{
		RemoveBuff(BuffRef);
	}
}

void UBuffComponent::AddBuff(ABuffInstance* BuffRef)
{
	if (!BuffRef || Buffs.Contains(BuffRef))return;

	Buffs.Add(BuffRef);
	OnAddBuff.Broadcast(BuffRef);
}

void UBuffComponent::RemoveBuff(ABuffInstance* BuffRef)
{
	if (!BuffRef || !Buffs.Contains(BuffRef))return;

	Buffs.Remove(BuffRef);
	OnRemoveBuff.Broadcast(BuffRef);
}

void UBuffComponent::AddBuffCount(FName Buff, int32 InCount)
{
	if (ABuffInstance* BuffRef = FindBuff(Buff))
	{
		BuffRef->AddCount(InCount);
	}
	else
	{
		PlayBuff(Buff);
	}
}

void UBuffComponent::RemoveBuffsInGroup(FName Group)
{
	for (ABuffInstance* BuffRef : Buffs)
	{
		if (BuffRef->BuffTableData.Group == Group)
		{
			BuffRef->Destroy();
		}
	}
}

