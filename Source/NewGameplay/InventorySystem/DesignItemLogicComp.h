// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventorySystem/InventoryItem.h"
#include "DesignItemLogicComp.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDesignItemLogicComp_OnUpdateParentItemSignature, AInventoryItem*, NewItem, AInventoryItem*, PrevItem);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable )
class NEWGAMEPLAY_API UDesignItemLogicComp : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDesignItemLogicComp();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Design Item Logic Component")
	bool bItemVisible = true;

	UPROPERTY(BlueprintReadOnly, Category = "Design Item Logic Component")
	AInventoryItem* ParentItem;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Design Item Logic Component")
	bool bInitDynamicMaterials;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Design Item Logic Component")
	bool bShowMultipleMesh;

	UFUNCTION(BlueprintCallable, Category = "Design Item Logic Component")
	void SetItemVisible(bool bVisible);

	UFUNCTION(BlueprintCallable, Category = "Design Item Logic Component")
	void UpdateVisible();

	UFUNCTION()
	void SetParentItem(AInventoryItem* Item);

	UFUNCTION(BlueprintCallable, Category = "Design Item Logic Component")
	void SetShowMultipleMesh(bool Value);

	UFUNCTION(BlueprintCallable, Category = "Design Item Logic Component")
	AActor* GetOwnerActor();

	UPROPERTY(BlueprintAssignable, Category = "Design Item Logic Component")
	FDesignItemLogicComp_OnUpdateParentItemSignature OnUpdateParentItem;
		
};
