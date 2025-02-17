// Fill out your copyright notice in the Description page of Project Settings.


#include "SpellSystem/SpellInstance.h"

// Sets default values
ASpellInstance::ASpellInstance()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASpellInstance::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASpellInstance::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASpellInstance::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (SpellsTable)
	{
		if (FSpellTableData* SpellRow = SpellsTable->FindRow<FSpellTableData>(SpellName, "Cant find spell row"))
		{
			SpellTableData = *SpellRow;
		}
	}
	
	SetLifeSpan(SpellTableData.Duration);
}

