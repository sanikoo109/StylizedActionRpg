// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySystem/ItemDesign_Skeletal.h"

// Sets default values
AItemDesign_Skeletal::AItemDesign_Skeletal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ItemMesh = CreateDefaultSubobject<USkeletalMeshComponent>("ItemMesh");

}

// Called when the game starts or when spawned
void AItemDesign_Skeletal::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AItemDesign_Skeletal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

