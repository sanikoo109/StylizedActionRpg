// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CombatSystem/CombatSystemTypes.h"
#include "InventorySystem/InventorySystemTypes.h"
#include "InventoryItem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FInventoryItem_OnUpdateSocketItemSignature, int32, Socket, AInventoryItem*, NewItem, AInventoryItem*, PrevItem);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FInventoryItem_OnUpdateSocketCountSignature, int32, NewCount, int32, PrevCount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FInventoryItem_OnUpdateCurrentLevelSignature, int32, NewLevel, int32, PrevLevel);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FInventoryItem_OnUpdateCountSignature, int32, NewCount, int32, PrevCount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FInventoryItem_OnUpdateParentTypeSignature, EInventoryItemParentType, NewType, EInventoryItemParentType, PrevType);

UCLASS()
class NEWGAMEPLAY_API AInventoryItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInventoryItem();

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Inventory Item")
	class UBuffComponent* BuffComponent;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Inventory Item")
	class UGamePropertyComponent* PropComponent_ItemBase;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Inventory Item")
	class UGamePropertyComponent* PropComponent_ItemSum;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Inventory Item")
	class UGamePropertyComponent* PropComponent_ItemBonus;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Inventory Item")
	class UGamePropertyComponent* PropComponent_Character;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Item|Socket")
	AInventoryItem* GetSocketItem(int32 SocketID);

	UFUNCTION(BlueprintCallable, Category = "Item|Socket")
	bool SetSocketItem(AInventoryItem* Item, int32 Socket);

	UFUNCTION()
	void SetSocketItem_Fast(AInventoryItem* Item, int32 Socket, AInventoryItem* PrevItem);

	UFUNCTION(BlueprintCallable, Category = "Item|Socket")
	void DropItemToSocket(AInventoryItem* Item, int32 SocketID);

	UFUNCTION(BlueprintCallable, Category = "Item|Socket")
	void SetSocketCount(int32 InCount);

	UFUNCTION(BlueprintCallable, Category = "Item|Socket")
	void AddSocketCount(int32 InCount);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Item|Socket")
	bool IsEmptySocket(int32 SocketID);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Item|Socket")
	bool IsEnableSocketItem(AInventoryItem* Item);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory Item")
	FGameDamage GetDamage();

	UFUNCTION(BlueprintCallable, BlueprintPure, BlueprintNativeEvent, Category = "Inventory Item")
	bool CanAttack();
	virtual bool CanAttack_Implementation() { return true; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory Item")
	AActor* GetOwnerActor();

	UPROPERTY(BlueprintReadOnly, Category = "Inventory Item")
	FInventoryItemTableData ItemTableData;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Inventory Item", meta = (ExposeOnSpawn = "true"))
	FName ItemRowName;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Inventory Item", meta = (ExposeOnSpawn = "true"))
	int32 Count;

	UPROPERTY(BlueprintReadOnly, Category = "Inventory Item")
	FInventoryItemParentData ParentData;

	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "Inventory Item")
	TArray<AInventoryItem*> SocketItems;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Inventory Item")
	int32 SocketCount;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Inventory Item")
	int32 CurrentLevel;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Inventory Item")
	FIntPoint Position;

	UPROPERTY(BlueprintReadOnly, Category = "Inventory Item")
	FGameDamage Damage;

	UFUNCTION()
	void UpdateDamage();

	UFUNCTION(BlueprintCallable, Category = "Inventory Item")
	void SetCount(int32 InCount);

	UFUNCTION(BlueprintCallable, Category = "Inventory Item")
	void AddCount(int32 InCount);

	UFUNCTION(BlueprintCallable, Category = "Inventory Item")
	void SetCurrentLevel(int32 InLevel);

	UFUNCTION(BlueprintCallable, Category = "Inventory Item")
	void AddCurrentLevel(int32 InLevel);

	UFUNCTION(BlueprintCallable, Category = "Inventory Item")
	bool CompleteItem(AInventoryItem* Item);

	UFUNCTION(BlueprintCallable, Category = "Inventory Item")
	void Transite(AInventoryItem* TargetItem, int32 InCount);

	UFUNCTION(BlueprintCallable, Category = "Inventory Item")
	AInventoryItem* Split(int32 InCount);

	UFUNCTION(BlueprintCallable, BlueprintPure, BlueprintNativeEvent, Category = "Inventory Item")
	UTexture2D* GetImage();
	virtual UTexture2D* GetImage_Implementation();

	UFUNCTION(BlueprintCallable, Category = "Inventory Item")
	void SetupParent(FInventoryItemParentData InData);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory Item")
	bool CanDropItem(AInventoryItem* Item);

	UFUNCTION(BlueprintCallable, Category = "Inventory Item")
	AInventoryItem* CopyItem(int32 InCount);

	UFUNCTION(BlueprintCallable, BlueprintPure, BlueprintNativeEvent, Category = "Inventory Item")
	bool CanReload();
	virtual bool CanReload_Implementation();

	UPROPERTY(BlueprintAssignable, Category = "Inventory Item")
	FInventoryItem_OnUpdateSocketItemSignature OnUpdateSocketItem;

	UPROPERTY(BlueprintAssignable, Category = "Inventory Item")
	FInventoryItem_OnUpdateSocketCountSignature OnUpdateSocketCount;

	UPROPERTY(BlueprintAssignable, Category = "Inventory Item")
	FInventoryItem_OnUpdateCurrentLevelSignature OnUpdateCurrentLevel;

	UPROPERTY(BlueprintAssignable, Category = "Inventory Item")
	FInventoryItem_OnUpdateCountSignature OnUpdateCount;

	UPROPERTY(BlueprintAssignable, Category = "Inventory Item")
	FInventoryItem_OnUpdateParentTypeSignature OnUpdateParentType;

	UFUNCTION()
	void OnUpdateSocketCount_Event(int32 NewCount, int32 PrevCount);

	UFUNCTION()
	void OnUpdateCount_Event(int32 NewCount, int32 PrevCount);

	UFUNCTION()
	void OnUpdateSocketItem_Event(int32 Socket, AInventoryItem* NewItem, AInventoryItem* PrevItem);

	UFUNCTION(BlueprintNativeEvent, Category = "Inventory Item")
	void OnUpdateParentType_Event(EInventoryItemParentType NewType, EInventoryItemParentType PrevType);
	virtual void OnUpdateParentType_Event_Implementation(EInventoryItemParentType NewType, EInventoryItemParentType PrevType) {};

	UFUNCTION()
	void OnUpdateCurrentLevel_Event(int32 NewLevel, int32 PrevLevel);

	UFUNCTION()
	void OnAddBuff_Event(class ABuffInstance* BuffRef);

	UFUNCTION()
	void OnRemoveBuff_Event(class ABuffInstance* BuffRef);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Inventory Item")
	void Use(class AMyCharacter* Char);

	UFUNCTION(BlueprintImplementableEvent, Category = "Inventory Item")
	void UseRelative(class UInventoryComponent* SourceInventory, class AInventoryItem* TargetItem);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Inventory Item")
	void Reload();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Inventory Item")
	void WasteAmmo(AInventoryItem* Item);
	virtual void WasteAmmo_Implementation(AInventoryItem* Item);

	UFUNCTION(BlueprintCallable, Category = "Inventory Item")
	void RemoveFromParent();

};
