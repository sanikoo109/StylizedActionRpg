// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InventorySystem/InventoryItem.h"
#include "InteractSystem/InteractInterface.h"
#include "PickUp.generated.h"

UCLASS()
class NEWGAMEPLAY_API APickUp : public AActor, public IInteractInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickUp();

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Pick Up")
	USceneComponent* RootScene;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Pick Up")
	UStaticMeshComponent* ItemMesh;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void Interact_Implementation(ACharacter* Char, UPrimitiveComponent* TargetComp) override;
	virtual FInteractInfo GetInteractInfo_Implementation(ACharacter* Char, UPrimitiveComponent* TargetComp) override;

	UPROPERTY(EditAnywhere, Category = "Pick Up")
	bool bUpdate;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Pick Up")
	FName Item;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Pick Up")
	int32 Count = 1;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Pick Up")
	AInventoryItem* InventoryItemRef;

	UFUNCTION(BlueprintNativeEvent, Category = "Pick Up")
	AInventoryItem* SpawnItem();
	virtual AInventoryItem* SpawnItem_Implementation();

};
