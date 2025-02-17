// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SpellSystem/SpellInstance.h"
#include "SpellComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FSpellComponent_OnUpdateHotBarSlotSignature, int32, ID, FName, NewSpell, FName, PrevSpell);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable )
class NEWGAMEPLAY_API USpellComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USpellComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY()
	class UCombatComponent* OwnerCombatComp;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Spell Component")
	TArray<FName> UnlockedSpells;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Spell Component")
	int32 HotBarSize;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Spell Component")
	TArray<FName> HotBarSlots;

	UPROPERTY(EditDefaultsOnly, Category = "Spell Component")
	UDataTable* SpellsTable;

	UPROPERTY(EditDefaultsOnly, Category = "Spell Component")
	TSubclassOf<ASpellInstance> BaseSpellInstanceClass;

	UFUNCTION(BlueprintCallable, Category = "Spell Component")
	void CastSpell(FName Spell, FTransform SpawnTransform, FTransform PreviewTransform);

	UFUNCTION(BlueprintCallable, Category = "Spell Component")
	void UnlockSpell(FName Spell);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Spell Component")
	FName GetHotBarSpell(int32 SlotID);

	UFUNCTION(BlueprintCallable, Category = "Spell Component")
	void SetHotBarSpell(int32 SlotID, FName Spell);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Spell Component")
	bool IsValidHotBarSlot(int32 SlotID);

	UFUNCTION(BlueprintCallable, Category = "Spell Component")
	void UseHotBarSpell(int32 SlotID);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Spell Component")
	bool IsUnlockedSpell(FName Spell);

	UPROPERTY(BlueprintAssignable, Category = "Spell Component")
	FSpellComponent_OnUpdateHotBarSlotSignature OnUpdateHotBarSlot;
		
};
