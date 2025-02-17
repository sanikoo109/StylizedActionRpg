// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BuffSystem/BuffSystemTypes.h"
#include "BuffInstance.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FBuffInstance_OnUpdateRemainingSignature, float, NewValue, float, PrevValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FBuffInstance_OnUpdateCountSignature, int32, NewCount, int32, PrevCount);

UCLASS(Blueprintable)
class NEWGAMEPLAY_API ABuffInstance : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABuffInstance();

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Buff")
	class UGamePropertyComponent* Properties_Base;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Buff")
	class UGamePropertyComponent* Properties_Sum;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void OnConstruction(const FTransform& Transform) override;

	UPROPERTY(EditAnywhere, Category = "Buff")
	UDataTable* BuffsTable;

	UPROPERTY(BlueprintReadOnly, Category = "Buff")
	FBuffTableData BuffTableData;

	UPROPERTY(BlueprintReadOnly, Category = "Buff")
	class UBuffComponent* ParentBuffComponent;

	UPROPERTY(BlueprintReadOnly, Category = "Buff")
	FName Buff;

	UPROPERTY(BlueprintReadOnly, Category = "Buff")
	float Remaining;

	UPROPERTY(BlueprintReadOnly, Category = "Buff")
	int32 Count = 1;

	UFUNCTION(BlueprintCallable, Category = "Buff")
	void SetRemaining(float Value);

	UFUNCTION(BlueprintCallable, Category = "Buff")
	void AddRemaining(float Value);

	UFUNCTION(BlueprintImplementableEvent, Category = "Buff")
	void UpdateBuff(float Delta);

	UFUNCTION()
	void OnUpdateRemaining_Event(float NewValue, float PrevValue);

	UFUNCTION()
	void UpdateRemaining(float DeltaTime);

	UFUNCTION(BlueprintCallable, Category = "Buff")
	void SetCount(int32 InCount);

	UFUNCTION(BlueprintCallable, Category = "Buff")
	void AddCount(int32 InCount);

	UFUNCTION()
	void OnUpdateCount_Event(int32 NewCount, int32 PrevCount);

	UFUNCTION()
	void UpdateSumProps();

	UPROPERTY(BlueprintAssignable, Category = "Buff")
	FBuffInstance_OnUpdateRemainingSignature OnUpdateRemaining;

	UPROPERTY(BlueprintAssignable, Category = "Buff")
	FBuffInstance_OnUpdateCountSignature OnUpdateCount;

};
