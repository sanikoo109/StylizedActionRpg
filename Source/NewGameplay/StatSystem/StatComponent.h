// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StatSystem/GameStatTypes.h"
#include "StatComponent.generated.h"

USTRUCT(BlueprintType)
struct FStatData
{

	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Stat Data")
	EStatList Stat = EStatList::ESL_None;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Stat Data")
	int32 Value = 0;

};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FStatComponent_OnUpdateStatValueSignature, EStatList, Stat, int32, NewValue, int32, PrevValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStatComponent_OnInvestPointSignature, EStatList, Stat);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NEWGAMEPLAY_API UStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Stat Component")
	TArray<FStatData> Stats;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Stat Component")
	int32 CurrentPoints;

	UFUNCTION(BlueprintCallable, Category = "Stat Component")
	void SetStatValue(EStatList Stat, int32 Value);

	UFUNCTION(BlueprintCallable, Category = "Stat Component")
	void AddStatValue(EStatList Stat, int32 Value);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Stat Component")
	int32 GetStatValue(EStatList Stat);

	UFUNCTION(BlueprintCallable, Category = "Stat Component")
	int32 FindStatID(EStatList Stat);

	UFUNCTION(BlueprintCallable, Category = "Stat Component")
	void SetCurrentPoints(int32 Value);

	UFUNCTION(BlueprintCallable, Category = "Stat Component")
	void AddCurrentPoints(int32 Value);

	UFUNCTION(BlueprintCallable, Category = "Stat Component")
	void InvestPoint(EStatList InStat);

	UPROPERTY(BlueprintAssignable, Category = "Stat Component")
	FStatComponent_OnUpdateStatValueSignature OnUpdateStatValue;

	UPROPERTY(BlueprintAssignable, Category = "Stat Component")
	FStatComponent_OnInvestPointSignature OnInvestPoint;

};
