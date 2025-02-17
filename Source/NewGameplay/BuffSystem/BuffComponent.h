// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BuffSystem/BuffInstance.h"
#include "BuffSystem/BuffSystemTypes.h"
#include "BuffComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBuffComponent_OnAddBuffSignature, ABuffInstance*, BuffRef);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBuffComponent_OnRemoveBuffSignature, ABuffInstance*, BuffRef);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class NEWGAMEPLAY_API UBuffComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBuffComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UPROPERTY(EditAnywhere, Category = "Buff Component")
	UDataTable* BuffsTable;

	UPROPERTY(BlueprintReadOnly, Category = "Buff Component")
	TArray<ABuffInstance*> Buffs;

	UPROPERTY(EditDefaultsOnly, Category = "Buff Component")
	TSubclassOf<ABuffInstance> BaseBuffInstanceClass;

	UFUNCTION(BlueprintCallable, Category = "Buff Component")
	void PlayBuff(FName Buff);
	
	UFUNCTION(BlueprintCallable, Category = "Buff Component")
	ABuffInstance* FindBuff(FName Buff);
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Buff Component")
	bool HaveBuff(FName Buff);

	UFUNCTION()
	void OnAddBuff_Event(ABuffInstance* BuffRef);
	
	UFUNCTION()
	void OnRemoveBuff_Event(ABuffInstance* BuffRef);
	
	UFUNCTION()
	void Buff_OnEndPlay_Event(AActor* Actor, EEndPlayReason::Type EndPlayReason);
	
	UFUNCTION(BlueprintCallable, Category = "Buff Component")
	void AddBuff(ABuffInstance* BuffRef);
	
	UFUNCTION(BlueprintCallable, Category = "Buff Component")
	void RemoveBuff(ABuffInstance* BuffRef);
	
	UFUNCTION(BlueprintCallable, Category = "Buff Component")
	void AddBuffCount(FName Buff, int32 InCount);
	
	UFUNCTION(BlueprintCallable, Category = "Buff Component")
	void RemoveBuffsInGroup(FName Group);
	
	UPROPERTY(BlueprintAssignable, Category = "Buff Component")
	FBuffComponent_OnAddBuffSignature OnAddBuff;
	
	UPROPERTY(BlueprintAssignable, Category = "Buff Component")
	FBuffComponent_OnRemoveBuffSignature OnRemoveBuff;


};
