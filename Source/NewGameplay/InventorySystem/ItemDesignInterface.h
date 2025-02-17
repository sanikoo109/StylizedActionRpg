// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ItemDesignInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UItemDesignInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class NEWGAMEPLAY_API IItemDesignInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Item Design Interface")
	UMeshComponent* I_ItemDesign_GetMesh();
	//virtual UMeshComponent* I_ItemDesign_GetMesh_Implementation() { return nullptr; }

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Item Design Interface")
	class UDesignItemLogicComp* I_ItemDesign_GetLogicComp();
	//virtual class UDesignItemLogicComp* I_ItemDesign_GetLogicComp_Implementation() { return nullptr; }

	UFUNCTION(BlueprintNativeEvent, Category = "Item Design Interface")
	void I_ItemDesign_UpdateVisible(bool IsVisible);
	//virtual void I_ItemDesign_UpdateVisible_Implementation(bool IsVisible) {};

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Item Design Interface")
	UMeshComponent* I_ItemDesign_GetAmmoMesh();
	//virtual UMeshComponent* I_ItemDesign_GetAmmoMesh_Implementation() { return nullptr; }

};
