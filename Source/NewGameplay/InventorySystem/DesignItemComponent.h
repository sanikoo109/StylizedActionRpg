// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DesignItemComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDesignItemComponent_OnUpdateItemSignature, AActor*, NewActor, AActor*, PrevActor);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable )
class NEWGAMEPLAY_API UDesignItemComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDesignItemComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintReadOnly, Category = "Design Item Component")
	AActor* CurrentItem;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Design Item Component")
	bool bAutoLeadPoseParent;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Design Item Component")
	bool bItemVisible = true;

	UFUNCTION(BlueprintCallable, Category = "Design Item Component")
	void SetItem(AActor* Item);

	UFUNCTION(BlueprintCallable, Category = "Design Item Component")
	void SetItemVisible(bool IsVisible);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Design Item Component")
	UMeshComponent* GetItemMesh();
	
	UPROPERTY(BlueprintAssignable, Category = "Design Item Component")
	FDesignItemComponent_OnUpdateItemSignature OnUpdateItem;

	UFUNCTION()
	void OnUpdateItem_Event(AActor* NewItem, AActor* PrevItem);

};
