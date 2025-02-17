// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Ladder.generated.h"

UCLASS()
class NEWGAMEPLAY_API ALadder : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALadder();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Ladder")
	FTransform GetStepLocation(int32 Step);

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ladder")
	float Width = 60.0f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ladder")
	float StepHeight = 50.0f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ladder")
	int32 StepCount = 5;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ladder")
	bool bCanExitDown = true;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ladder")
	bool bCanExitUp = true;

};
