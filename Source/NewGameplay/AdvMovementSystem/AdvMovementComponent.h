// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AdvMovementComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAdvMovementComponent_OnUpdateSprintSignature, bool, IsSprint);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAdvMovementComponent_OnUpdateSprintInputSignature, bool, IsSprintInputActive);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NEWGAMEPLAY_API UAdvMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAdvMovementComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Adv Movement")
	void Sprint();
	
	UFUNCTION(BlueprintCallable, Category = "Adv Movement")
	void Unsprint();

	UFUNCTION(BlueprintCallable, Category = "Adv Movement")
	void CheckSprint();

	UPROPERTY(BlueprintReadOnly, Category = "Adv Movement")
	bool bSprintInput;

	UPROPERTY(BlueprintReadOnly, Category = "Adv Movement")
	bool bSprint;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Adv Movement")
	float WalkSpeed = 300.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Adv Movement")
	float SprintSpeed = 600.0f;

	UPROPERTY(BlueprintAssignable, Category = "Adv Movement")
	FAdvMovementComponent_OnUpdateSprintInputSignature OnUpdateSprintInput;

	UPROPERTY(BlueprintAssignable, Category = "Adv Movement")
	FAdvMovementComponent_OnUpdateSprintSignature OnUpdateSprint;

};
