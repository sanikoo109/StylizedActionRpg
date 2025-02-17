// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributeSystem/GameAttributeTypes.h"
#include "AttributeComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FAttributeComponent_OnUpdateCurrentValueSignature, EAttributeList, Attribute, float, NewValue, float, PrevValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FAttributeComponent_OnUpdateMaxValueSignature, EAttributeList, Attribute, float, NewValue, float, PrevValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAttributeComponent_OnEmptySignature, EAttributeList, Attribute);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAttributeComponent_OnFullSignature, EAttributeList, Attribute);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NEWGAMEPLAY_API UAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAttributeComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attribute Component")
	TArray<FAttributeData> Attributes;

	UFUNCTION(BlueprintCallable, Category = "Attribute Component")
	void AddCurrentValue(EAttributeList Attribute, float InValue);

	UFUNCTION(BlueprintCallable, Category = "Attribute Component")
	void SetCurrentValue(EAttributeList Attribute, float InValue);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Attribute Component")
	float GetCurrentValue(EAttributeList Attribute);

	UFUNCTION(BlueprintCallable, Category = "Attribute Component")
	FAttributeData GetAttributeData(EAttributeList Attribute);

	UFUNCTION(BlueprintCallable, Category = "Attribute Component")
	int32 FindAttributeID(EAttributeList Attribute);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Attribute Component")
	float GetMaxValue(EAttributeList Attribute);

	UFUNCTION(BlueprintCallable, Category = "Attribute Component")
	void SetMaxValue(EAttributeList Attribute, float InValue);

	UFUNCTION(BlueprintCallable, Category = "Attribute Component")
	void AddMaxValue(EAttributeList Attribute, float InValue);

	UPROPERTY(BlueprintAssignable, Category = "Attribute Component")
	FAttributeComponent_OnUpdateCurrentValueSignature OnUpdateCurrentValue;

	UPROPERTY(BlueprintAssignable, Category = "Attribute Component")
	FAttributeComponent_OnUpdateMaxValueSignature OnUpdateMaxValue;

	UPROPERTY(BlueprintAssignable, Category = "Attribute Component")
	FAttributeComponent_OnEmptySignature OnEmpty;

	UPROPERTY(BlueprintAssignable, Category = "Attribute Component")
	FAttributeComponent_OnFullSignature OnFull;

	UFUNCTION()
	void OnUpdateCurrentValue_Event(EAttributeList Attribute, float NewValue, float PrevValue);

};
