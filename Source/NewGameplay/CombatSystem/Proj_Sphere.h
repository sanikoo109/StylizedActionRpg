// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CombatSystem/GameDamageTypes.h"

#include "Proj_Sphere.generated.h"

UCLASS()
class NEWGAMEPLAY_API AProj_Sphere : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProj_Sphere();

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Proj Sphere")
	class USphereComponent* Sphere;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Proj Sphere")
	USceneComponent* PreviewPivot;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Proj Sphere")
	UStaticMeshComponent* PreviewMesh;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Proj Sphere")
	class UProjectileMovementComponent* ProjectileMovement;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadOnly, Category = "Proj Sphere", meta = (ExposeOnSpawn = "true"))
	float Radius = 10.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Proj Sphere", meta = (ExposeOnSpawn = "true"))
	FGameDamage Damage;

	UPROPERTY(BlueprintReadOnly, Category = "Proj Sphere")
	FVector StartLocation;

	UPROPERTY(BlueprintReadOnly, Category = "Proj Sphere", meta = (ExposeOnSpawn = "true"))
	bool bUsePreviewTransform;

	UPROPERTY(BlueprintReadOnly, Category = "Proj Sphere", meta = (ExposeOnSpawn = "true"))
	FTransform PreviewTransform;

	UPROPERTY(BlueprintReadOnly, Category = "Proj Sphere", meta = (ExposeOnSpawn = "true"))
	float SpeedScale = 1.0f;

	UFUNCTION()
	void OnProjectileStop_Event(const FHitResult& ImpactResult);

	UFUNCTION(BlueprintImplementableEvent, Category = "Proj Sphere")
	void HitEffect();

};
