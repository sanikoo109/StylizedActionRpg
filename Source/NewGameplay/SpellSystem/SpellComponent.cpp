// Fill out your copyright notice in the Description page of Project Settings.


#include "SpellSystem/SpellComponent.h"
#include "SpellSystem/SpellSystemTypes.h"
#include "SpellSystem/SpellInstance.h"
#include "CombatSystem/CombatComponent.h"

// Sets default values for this component's properties
USpellComponent::USpellComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USpellComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	OwnerCombatComp = GetOwner()->FindComponentByClass<UCombatComponent>();
}


// Called every frame
void USpellComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void USpellComponent::CastSpell(FName Spell, FTransform SpawnTransform, FTransform PreviewTransform)
{
	if (!IsUnlockedSpell(Spell))return;

	if (SpellsTable)
	{
		if (FSpellTableData* SpellRow = SpellsTable->FindRow<FSpellTableData>(Spell, "Cant find spell row"))
		{
			TSubclassOf<ASpellInstance> SpellInstanceClass = BaseSpellInstanceClass;
			if (SpellRow->InstanceClass != NULL)SpellInstanceClass = SpellRow->InstanceClass;
			APawn* Instigator = Cast<APawn>(GetOwner());
			ASpellInstance* SpawnedSpell = GetWorld()->SpawnActorDeferred<ASpellInstance>(SpellInstanceClass, SpawnTransform, nullptr, Instigator, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
			SpawnedSpell->SpellName = Spell;
			SpawnedSpell->PreviewTransform = PreviewTransform;
			SpawnedSpell->ParentSpellComp = this;
			SpawnedSpell->FinishSpawning(SpawnTransform);
		}
	}
}

void USpellComponent::UnlockSpell(FName Spell)
{
	UnlockedSpells.AddUnique(Spell);
}

FName USpellComponent::GetHotBarSpell(int32 SlotID)
{
	if (HotBarSlots.IsValidIndex(SlotID))
	{
		return HotBarSlots[SlotID];
	}
	return FName();
}

void USpellComponent::SetHotBarSpell(int32 SlotID, FName Spell)
{
	if (!IsValidHotBarSlot(SlotID))return;

	FName PrevSpell = GetHotBarSpell(SlotID);
	if (PrevSpell != Spell)
	{
		HotBarSlots.SetNum(SlotID + 1, EAllowShrinking::No);
		HotBarSlots[SlotID] = Spell;
		OnUpdateHotBarSlot.Broadcast(SlotID, Spell, PrevSpell);
	}
}

bool USpellComponent::IsValidHotBarSlot(int32 SlotID)
{
	return SlotID >= 0 && SlotID < HotBarSize;
}

void USpellComponent::UseHotBarSpell(int32 SlotID)
{
	FName FindedSpell = GetHotBarSpell(SlotID);
	if (FindedSpell != "")
	{
		OwnerCombatComp->Attack(FindedSpell);
	}
}

bool USpellComponent::IsUnlockedSpell(FName Spell)
{
	return UnlockedSpells.Contains(Spell);
}
