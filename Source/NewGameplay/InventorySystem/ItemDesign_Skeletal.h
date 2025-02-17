// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemDesign_Skeletal.generated.h"

UCLASS()
class NEWGAMEPLAY_API AItemDesign_Skeletal : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemDesign_Skeletal();

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Item Design Skeletal")
	USkeletalMeshComponent* ItemMesh;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
