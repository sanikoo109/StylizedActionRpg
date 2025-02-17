// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ExperienceComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FExperienceComponent_OnLevelUpSignature);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NEWGAMEPLAY_API UExperienceComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UExperienceComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Experience Component")
	int32 CurrentLevel;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Experience Component")
	int32 MaxLevel = 30;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Experience Component")
	int32 CurrentExperience;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Experience Component")
	UCurveFloat* ExperienceCurve;
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Experience Component")
	int32 GetMaxExperience();

	UFUNCTION(BlueprintCallable, Category = "Experience Component")
	void AddExperience(int32 Value);

	UPROPERTY(BlueprintAssignable, Category = "Experience Component")
	FExperienceComponent_OnLevelUpSignature OnLevelUp;

};
