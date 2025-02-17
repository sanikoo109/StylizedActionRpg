// Fill out your copyright notice in the Description page of Project Settings.


#include "AdvMovementSystem/AdvMovementComponent.h"
#include "AdvMovementComponent.h"

// Sets default values for this component's properties
UAdvMovementComponent::UAdvMovementComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UAdvMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UAdvMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CheckSprint();
}

void UAdvMovementComponent::Sprint()
{
	if (!bSprintInput)
	{
		bSprintInput = true;
		OnUpdateSprintInput.Broadcast(true);
	}
}

void UAdvMovementComponent::Unsprint()
{
	if (bSprintInput)
	{
		bSprintInput = false;
		OnUpdateSprintInput.Broadcast(false);
	}
}

void UAdvMovementComponent::CheckSprint()
{
	bool TargetSprint = false;
	if (bSprintInput)
	{
		FVector OwnerVelocity = GetOwner()->GetVelocity();
		FVector OwnerVelocityXY = FVector(OwnerVelocity.X, OwnerVelocity.Y, 0.0f);
		float OwnerSpeed = OwnerVelocityXY.Length();
		float MinSprintSpeed = (WalkSpeed + SprintSpeed) / 2;
		if (OwnerSpeed >= MinSprintSpeed)
		{
			TargetSprint = true;
		}
	}

	if (TargetSprint != bSprint)
	{
		bSprint = TargetSprint;
		OnUpdateSprint.Broadcast(bSprint);
	}
}

