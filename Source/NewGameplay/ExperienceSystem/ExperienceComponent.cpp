// Fill out your copyright notice in the Description page of Project Settings.


#include "ExperienceSystem/ExperienceComponent.h"

// Sets default values for this component's properties
UExperienceComponent::UExperienceComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UCurveFloat> CurveObject(TEXT("/Game/NewGameplay/Systems/ExperienceSystem/ExperienceLevelCurve.ExperienceLevelCurve"));
	if (CurveObject.Succeeded())
	{
		ExperienceCurve = CurveObject.Object;
	}
}


// Called when the game starts
void UExperienceComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UExperienceComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

int32 UExperienceComponent::GetMaxExperience()
{
	return ExperienceCurve->GetFloatValue(CurrentLevel);
}

void UExperienceComponent::AddExperience(int32 Value)
{
	CurrentExperience += Value;
	while (CurrentExperience > GetMaxExperience())
	{
		CurrentExperience -= GetMaxExperience();
		CurrentLevel++;
		OnLevelUp.Broadcast();
	}
}

