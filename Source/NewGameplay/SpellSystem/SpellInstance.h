// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpellSystem/SpellSystemTypes.h"
#include "SpellInstance.generated.h"

UCLASS()
class NEWGAMEPLAY_API ASpellInstance : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpellInstance();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void OnConstruction(const FTransform& Transform) override;

	UPROPERTY(EditDefaultsOnly, Category = "Spell Instance")
	UDataTable* SpellsTable;

	UPROPERTY(BlueprintReadOnly, Category = "Spell Instance")
	class USpellComponent* ParentSpellComp;

	UPROPERTY(BlueprintReadOnly, Category = "Spell Instance")
	FName SpellName;

	UPROPERTY(BlueprintReadOnly, Category = "Spell Instance")
	FSpellTableData SpellTableData;

	UPROPERTY(BlueprintReadOnly, Category = "Spell Instance")
	FTransform PreviewTransform;

};
