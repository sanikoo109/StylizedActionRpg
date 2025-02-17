// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractSystem/InteractComponent.h"
#include "NewGameplay.h"
#include "GameFramework/Character.h"

// Sets default values for this component's properties
UInteractComponent::UInteractComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UInteractComponent::BeginPlay()
{
	Super::BeginPlay();

	ParentChar = Cast<ACharacter>(GetOwner());
}


// Called every frame
void UInteractComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	InteractInfo = FInteractInfo();

	FHitResult HitResult;
	InteractTrace(HitResult);
	if (AActor* HitActor = HitResult.GetActor())
	{
		if (HitActor->Implements<UInteractInterface>())
		{
			InteractInfo = IInteractInterface::Execute_GetInteractInfo(HitActor, ParentChar, HitResult.GetComponent());
		}
	}
}

void UInteractComponent::InteractTrace(FHitResult& HitResult)
{
	FVector Start = GetComponentLocation();
	FVector End = Start + (GetForwardVector() * TraceLength);
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(GetOwner());
	GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Interact, QueryParams);
}

void UInteractComponent::Interact()
{
	FHitResult HitResult;
	InteractTrace(HitResult);
	if (AActor* HitActor = HitResult.GetActor())
	{
		if (HitActor->Implements<UInteractInterface>())
		{
			IInteractInterface::Execute_Interact(HitActor, ParentChar, HitResult.GetComponent());
		}
	}
}

