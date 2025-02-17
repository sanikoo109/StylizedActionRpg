// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GamePropertySystem/GamePropertyTypes.h"
#include "GamePropertyComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FGamePropertyComponent_OnUpdatePropValueSignature, FName, Prop, float, NewValue, float, PrevValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGamePropertyComponent_OnRegisterChildPropsSignature, UGamePropertyComponent*, PropComp);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGamePropertyComponent_OnUnregisterChildPropsSignature, UGamePropertyComponent*, PropComp);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FGamePropertyComponent_OnUpdatePropActiveSignature, FName, Prop, bool, IsActive);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FGamePropertyComponent_OnUpdatePropPositiveSignature, FName, Prop, bool, IsPositive);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class NEWGAMEPLAY_API UGamePropertyComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGamePropertyComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Game Property Component")
	TArray<FGamePropertyData> Props;

	UPROPERTY(BlueprintReadOnly, Category = "Game Property Component")
	TArray<UGamePropertyComponent*> ChildrenProps;

	UFUNCTION(BlueprintCallable, Category = "Game Property Component")
	void SetPropValue(FName Prop, float Value);

	UFUNCTION(BlueprintCallable, Category = "Game Property Component")
	void AddPropValue(FName Prop, float Value);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Game Property Component")
	float GetPropValue(FName Prop);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Game Property Component")
	bool IsPropActive(FName Prop);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Game Property Component")
	bool IsPropPositive(FName Prop);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Game Property Component")
	bool IsPropNegative(FName Prop);

	UFUNCTION(BlueprintCallable, Category = "Game Property Component")
	FGamePropertyData GetPropData(FName Prop);

	UFUNCTION(BlueprintCallable, Category = "Game Property Component")
	int32 FindPropID(FName Prop);

	UFUNCTION(BlueprintCallable, Category = "Game Property Component")
	void RegisterChildProps(UGamePropertyComponent* PropComp);

	UFUNCTION(BlueprintCallable, Category = "Game Property Component")
	void UnregisterChildProps(UGamePropertyComponent* PropComp);

	UFUNCTION(BlueprintCallable, Category = "Game Property Component")
	void UpdateProp(FName Prop);

	UFUNCTION()
	void OnRegisterChildProps_Event(UGamePropertyComponent* PropComp);

	UFUNCTION()
	void OnUnregisterChildProps_Event(UGamePropertyComponent* PropComp);

	UFUNCTION()
	void OnUpdatePropValue_Event(FName Prop, float NewValue, float PrevValue);

	UFUNCTION()
	void ChildProps_OnUpdatePropValue_Event(FName Prop, float NewValue, float PrevValue);

	UPROPERTY(BlueprintAssignable, Category = "Game Property Component")
	FGamePropertyComponent_OnUpdatePropValueSignature OnUpdatePropValue;

	UPROPERTY(BlueprintAssignable, Category = "Game Property Component")
	FGamePropertyComponent_OnRegisterChildPropsSignature OnRegisterChildProps;

	UPROPERTY(BlueprintAssignable, Category = "Game Property Component")
	FGamePropertyComponent_OnUnregisterChildPropsSignature OnUnregisterChildProps;

	UPROPERTY(BlueprintAssignable, Category = "Game Property Component")
	FGamePropertyComponent_OnUpdatePropActiveSignature OnUpdatePropActive;

	UPROPERTY(BlueprintAssignable, Category = "Game Property Component")
	FGamePropertyComponent_OnUpdatePropPositiveSignature OnUpdatePropPositive;

};
