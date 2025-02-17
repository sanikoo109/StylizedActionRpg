// Fill out your copyright notice in the Description page of Project Settings.


#include "Assets/Ladder.h"

// Sets default values
ALadder::ALadder()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ALadder::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALadder::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FTransform ALadder::GetStepLocation(int32 Step)
{
	FTransform RelativeTransform = FTransform(FRotator(), FVector(0.0f, 0.0f, Step * StepHeight), FVector::OneVector);
	return RelativeTransform * GetActorTransform();
}

