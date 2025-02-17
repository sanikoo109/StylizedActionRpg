// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "InteractInterface.h"
#include "InteractComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NEWGAMEPLAY_API UInteractComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInteractComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Interact Component")
	float TraceLength = 300.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Interact Component")
	FInteractInfo InteractInfo;

	UFUNCTION(BlueprintCallable, Category = "Interact Component")
	void InteractTrace(FHitResult& HitResult);

	UFUNCTION(BlueprintCallable, Category = "Interact Component")
	void Interact();

protected:

	UPROPERTY()
	class ACharacter* ParentChar;

};
