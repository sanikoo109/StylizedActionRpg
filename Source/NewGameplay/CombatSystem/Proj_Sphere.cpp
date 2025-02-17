// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatSystem/Proj_Sphere.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "CombatSystem/GameDamageInterface.h"
#include "CombatSystem/CombatSystemLibrary.h"

// Sets default values
AProj_Sphere::AProj_Sphere()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	RootComponent = Sphere;
	Sphere->SetSphereRadius(25.0f);
	Sphere->SetCollisionProfileName("NoCollision");

	PreviewPivot = CreateDefaultSubobject<USceneComponent>("PreviewPivot");
	PreviewPivot->SetupAttachment(RootComponent);

	PreviewMesh = CreateDefaultSubobject<UStaticMeshComponent>("PreviewMesh");
	PreviewMesh->SetupAttachment(PreviewPivot);
	PreviewMesh->SetCollisionProfileName("NoCollision");

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	ProjectileMovement->InitialSpeed = 5000.0f;
	ProjectileMovement->bRotationFollowsVelocity = true;
}

// Called when the game starts or when spawned
void AProj_Sphere::BeginPlay()
{
	Super::BeginPlay();
	
	StartLocation = GetActorLocation();

	Sphere->IgnoreActorWhenMoving(GetInstigator(), true);
	Sphere->SetSphereRadius(Radius);
	Sphere->SetCollisionProfileName("Projectile");

	ProjectileMovement->OnProjectileStop.AddDynamic(this, &AProj_Sphere::OnProjectileStop_Event);
}

void AProj_Sphere::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	//Sphere->SetSphereRadius(Radius);
	//ProjectileMovement->InitialSpeed *= SpeedScale;
	if (bUsePreviewTransform)
	{
		FTransform LocalTransform = PreviewMesh->GetRelativeTransform().Inverse() * PreviewTransform;
		PreviewPivot->SetWorldTransform(LocalTransform, false, (FHitResult*)nullptr, ETeleportType::TeleportPhysics);
	}
}

// Called every frame
void AProj_Sphere::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FTransform InterpTransform = UKismetMathLibrary::TInterpTo(PreviewPivot->GetRelativeTransform(), FTransform(), DeltaTime, 10.0f);
	PreviewPivot->SetRelativeTransform(InterpTransform, false, (FHitResult*)nullptr, ETeleportType::TeleportPhysics);
}

void AProj_Sphere::OnProjectileStop_Event(const FHitResult& ImpactResult)
{

	if (AActor* HitActor = ImpactResult.GetActor())
	{
		if (HitActor->Implements<UGameDamageInterface>())
		{
			FVector HitFromDirection = StartLocation - ImpactResult.GetActor()->GetActorLocation();
			HitFromDirection.Normalize();

			FGameDamage LocalDamage = UCombatSystemLibrary::ScaleDamage(Damage, SpeedScale);

			FGameDamage OutDamage;
			IGameDamageInterface::Execute_ApplyGameDamage(ImpactResult.GetActor(), LocalDamage, GetInstigatorController(), this, true, HitFromDirection, ImpactResult, OutDamage);
		}
	}

	HitEffect();
	Destroy();
}

