// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyCharacter.generated.h"

UCLASS()
class NEWGAMEPLAY_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "My Character")
	void Align(FTransform Point, float Time = 0.35f);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "My Character")
	void ForceAlignEnd();

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "My Character")
	bool bEnemy;

	UFUNCTION(BlueprintCallable, BlueprintPure, BlueprintImplementableEvent, Category = "My Character")
	bool IsDead();

};
