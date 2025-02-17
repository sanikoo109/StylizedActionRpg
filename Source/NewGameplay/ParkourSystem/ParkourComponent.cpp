// Fill out your copyright notice in the Description page of Project Settings.


#include "ParkourSystem/ParkourComponent.h"
#include "MyCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Assets/Ladder.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "NewGameplay.h"

// Sets default values for this component's properties
UParkourComponent::UParkourComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	bAutoActivate = true;

	Ladder_Offset = FTransform(FRotator(), FVector(-50.0f, 0.0f, 88.0f), FVector::OneVector);
	LedgeHang_CornerOffset = FTransform(FRotator(), FVector(-46.0f, 0.0f, -88.0f), FVector::OneVector);
	LedgeHang_CornerMoveTestOffset = FTransform(FRotator(), FVector(5.0f, 0.0f, 5.0f), FVector::OneVector);

	for (int i = 0; i <= 15; ++i)
	{
		LedgeHang_TraceHandsHeightList.Add(i * 10.0f);
	}
}


// Called when the game starts
void UParkourComponent::BeginPlay()
{
	Super::BeginPlay();

	OnComponentActivated.AddDynamic(this, &UParkourComponent::OnComponentActivated_Event);
	OnComponentDeactivated.AddDynamic(this, &UParkourComponent::OnComponentDeactivated_Event);
	OnUpdateParkourMode.AddDynamic(this, &UParkourComponent::OnUpdateParkourMode_Event);

	if (!IsActive())
	{
		SetComponentTickEnabled(false);
	}

	ParentCharacter = Cast<AMyCharacter>(GetOwner());
	CapsuleDefaultHalfHeight = ParentCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
}


// Called every frame
void UParkourComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (ParkourMode == EParkourMode::EPM_None)
	{
		CheckLedge();
	}
	if (ParkourMode == EParkourMode::EPM_LedgeHang)
	{
		UpdateLedgeMove();
	}

	if (bUpdateHeight)
	{
		UpdateHeightByAnimCurve();
	}
}

void UParkourComponent::SetUpdatingHeight(bool Value)
{
	if (bUpdateHeight != Value)
	{
		bUpdateHeight = Value;
		UpdateHeightByAnimCurve();
	}
}

void UParkourComponent::BeginLadder(ALadder* LadderRef, int32 Step)
{
	if (!LadderRef || CurrentLadder)return;
	CurrentLadder = LadderRef;
	Ladder_Step = ClampLadderStep(LadderRef, Step);
	SetParkourMode(EParkourMode::EPM_Ladder);
	ParentCharacter->Align(Ladder_Offset * CurrentLadder->GetStepLocation(Ladder_Step));
	BeginLadder_Event();
}

void UParkourComponent::LadderAnimEnded()
{
	SetParkourMode(EParkourMode::EPM_None);
}

void UParkourComponent::UpdateLadderPos()
{
	Teleport(Ladder_Offset * CurrentLadder->GetStepLocation(Ladder_Step));
}

void UParkourComponent::LadderMove(bool bUp)
{
	if (IsLadderBusy())return;
	int32 TargetStep = Ladder_Step + (bUp ? 1 : -1);
	if (TargetStep < 0)
	{
		if (CurrentLadder->bCanExitDown)
		{
			Ladder_ExitDown();
		}
		return;
	}
	if (TargetStep > GetMaxLadderStep(CurrentLadder))
	{
		if (CurrentLadder->bCanExitUp)
		{
			Ladder_ExitUp();
		}
		return;
	}
	Ladder_Step = TargetStep;
	Ladder_PlayStepAnim(bUp);
}

int32 UParkourComponent::ClampLadderStep(ALadder* Ladder, int32 InStep)
{
	if (!Ladder)return 0;
	return FMath::Clamp(InStep, 0, GetMaxLadderStep(Ladder));
}

void UParkourComponent::LadderJumpOff()
{
	LadderOff();

	FVector LaunchVelocity;
	FRotator CharRotYaw = FRotator(0.0f, ParentCharacter->GetControlRotation().Yaw, 0.0f);
	FVector CharVectorFwd = CharRotYaw.RotateVector(FVector::ForwardVector);

	LaunchVelocity = CharVectorFwd * Ladder_JumpOffXYPower;
	LaunchVelocity.Z = ParentCharacter->GetCharacterMovement()->JumpZVelocity;
	
	ParentCharacter->LaunchCharacter(LaunchVelocity, false, false);
}

void UParkourComponent::LadderOff()
{
	ParentCharacter->SetActorRotation(FRotator(0.0f, ParentCharacter->GetControlRotation().Yaw, 0.0f), ETeleportType::TeleportPhysics);
	Ladder_StopMontage();
}

int32 UParkourComponent::GetMaxLadderStep(ALadder* Ladder)
{
	return Ladder->StepCount - Ladder_StepHeight - 1;
}

void UParkourComponent::BeginLedgeHang(FVector CornerLocation, FRotator CornerRotation)
{
	if (ParkourMode != EParkourMode::EPM_LedgeHang)
	{
		ParentCharacter->Align(LedgeHang_CornerOffset * FTransform(CornerRotation, CornerLocation, FVector::OneVector));
		SetParkourMode(EParkourMode::EPM_LedgeHang);
	}
}

bool UParkourComponent::LedgeHang_TraceFloor(FVector ImpactPoint, FVector ImpactNormal, FVector& FloorLocation)
{
	FloorLocation = FVector();

	TArray<FHitResult> HitResultList;
	FVector TraceStart;
	FVector TraceEnd;

	TraceEnd = ImpactPoint + (ImpactNormal * LedgeHang_TraceDepth * -1);
	TraceStart = FVector(TraceEnd.X, TraceEnd.Y, ParentCharacter->GetActorLocation().Z + LedgeHang_TraceHeight);

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(ParentCharacter);
	GetWorld()->LineTraceMultiByChannel(HitResultList, TraceStart, TraceEnd, ECC_Parkour, QueryParams);

	for (FHitResult HitResult : HitResultList)
	{
		if (!HitResult.bStartPenetrating)
		{
			FVector CornerPoint = FVector(ImpactPoint.X, ImpactPoint.Y, HitResult.ImpactPoint.Z);
			if (LedgeHang_HandTrace(HitResult.ImpactPoint, CornerPoint, ImpactNormal) && 
				LedgeHang_CapsuleTrace(CornerPoint, ImpactNormal))
			{
				FloorLocation = HitResult.ImpactPoint;
				return true;
			}
		}
	}

	return false;
}

bool UParkourComponent::LedgeHang_CapsuleTrace(FVector CornerPoint, FVector CornerNormal)
{
	float CapsuleRadius = ParentCharacter->GetCapsuleComponent()->GetScaledCapsuleRadius();
	float CapsuleHalfHeight = ParentCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

	FHitResult HitResult;
	FVector TraceStart;
	FVector TraceEnd;

	FVector CharCornerLoc = CornerPoint + (CornerNormal * CapsuleRadius);

	TraceStart = FVector(CharCornerLoc.X, CharCornerLoc.Y, ParentCharacter->GetActorLocation().Z);
	TraceEnd = FVector(CharCornerLoc.X, CharCornerLoc.Y, CharCornerLoc.Z - CapsuleHalfHeight + (LedgeHang_HandsRadius * 2));

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(ParentCharacter);

	GetWorld()->SweepSingleByChannel(HitResult, TraceStart, TraceEnd, FRotator().Quaternion(), ECC_Visibility,
		FCollisionShape::MakeCapsule(CapsuleRadius - 1, CapsuleHalfHeight - 1), QueryParams);

	return !HitResult.bBlockingHit;
}

bool UParkourComponent::LedgeHang_HandTrace(FVector GrabPoint, FVector CornerPoint, FVector CornerNormal)
{
	float CapsuleRadius = ParentCharacter->GetCapsuleComponent()->GetScaledCapsuleRadius();
	float CapsuleHalfHeight = ParentCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

	FHitResult HitResult;
	FVector TraceStart;
	FVector TraceEnd;

	FVector CharCornerLoc = CornerPoint + (CornerNormal * CapsuleRadius);

	TraceStart = FVector(CharCornerLoc.X, CharCornerLoc.Y, ParentCharacter->GetActorLocation().Z);
	TraceEnd = FVector(CharCornerLoc.X, CharCornerLoc.Y, CharCornerLoc.Z + LedgeHang_HandsRadius);

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(ParentCharacter);

	FCollisionShape SphereShape = FCollisionShape::MakeSphere(LedgeHang_HandsRadius - 1);

	GetWorld()->SweepSingleByChannel(HitResult, TraceStart, TraceEnd, FRotator().Quaternion(), ECC_Visibility, SphereShape, QueryParams);

	if (HitResult.bBlockingHit)return false;

	TraceStart = HitResult.TraceEnd;
	TraceEnd = CornerPoint + (CornerNormal * LedgeHang_HandsRadius * -1.0f) + FVector(0.0f, 0.0f, LedgeHang_HandsRadius);

	GetWorld()->SweepSingleByChannel(HitResult, TraceStart, TraceEnd, FRotator().Quaternion(), ECC_Visibility, SphereShape, QueryParams);

	return !HitResult.bBlockingHit;
}

void UParkourComponent::LedgeHangOff()
{
	ParentCharacter->SetActorRotation(FRotator(0.0f,ParentCharacter->GetControlRotation().Yaw, 0.0f), ETeleportType::TeleportPhysics);
	SetParkourMode(EParkourMode::EPM_None);
}

void UParkourComponent::LedgeHangJumpOff()
{
	if (LedgeHang_bWall)
	{
		LedgeHangOff();

		FVector LaunchVelocity;

		FRotator CharYawRot = FRotator(0.0f, ParentCharacter->GetControlRotation().Yaw, 0.0f);
		FVector CharVectorFwd = CharYawRot.RotateVector(FVector::ForwardVector);
		LaunchVelocity = CharVectorFwd * Ladder_JumpOffXYPower;
		LaunchVelocity.Z = ParentCharacter->GetCharacterMovement()->JumpZVelocity;

		ParentCharacter->LaunchCharacter(LaunchVelocity, false, false);
	}
}

void UParkourComponent::LedgeHangMove(bool bRight)
{
	if (IsLedgeHangBusy() || !LedgeHang_MoveTest(bRight))return;

	LedgeHang_bMove = true;
	LedgeHang_bMoveRight = bRight;
}

bool UParkourComponent::IsLedgeHangBusy()
{
	return false;
}

void UParkourComponent::LedgeHangUp()
{
	if (IsPlayingLedgeHangUpAnim())return;

	FVector CornerLoc = LedgeHang_GetCornerTransform().GetLocation();
	if (LedgeHang_UpCapsuleTrace(CornerLoc))
	{
		PlayMontage_LedgeHangUp();
		return;
	}
	if (LedgeHang_UpCrouchCapsuleTrace(CornerLoc))
	{
		PlayMontage_LedgeHangUpCrouch();
		return;
	}
}

void UParkourComponent::EndMontage_LedgeHangUp()
{
	SetParkourMode(EParkourMode::EPM_None);
}

void UParkourComponent::EndMontage_LedgeHangUpCrouch()
{
	SetParkourMode(EParkourMode::EPM_None);
	bUpdateHeight = false;
}

void UParkourComponent::LedgeHangUpCrouch_UpdateState(int32 Frame)
{
	if (Frame == 0)
	{
		SetParkourMode(EParkourMode::EPM_None);
		bUpdateHeight = false;
	}
	if (Frame == 1)
	{
		ParentCharacter->Crouch();
	}
	if (Frame == 2)
	{
		ParentCharacter->UnCrouch();
	}
}

bool UParkourComponent::LedgeHang_MoveTest(bool bRight)
{
	return LedgeHang_MoveCornerTest(bRight) &&
		LedgeHang_MoveHoleTest(bRight) &&
		LedgeHang_MoveCapsuleTest(bRight);
}

bool UParkourComponent::LedgeHang_MoveCapsuleTest(bool bRight)
{
	float CapsuleRadius = ParentCharacter->GetCapsuleComponent()->GetScaledCapsuleRadius();
	float CapsuleHalfHeight = ParentCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

	FHitResult HitResult;
	FVector TraceStart;
	FVector TraceEnd;

	TraceStart = ParentCharacter->GetActorLocation();
	TraceEnd = TraceStart + (ParentCharacter->GetActorRightVector() * LedgeHang_MoveTestDistance * (bRight ? 1.0f : -1.0f));

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(ParentCharacter);

	GetWorld()->SweepSingleByChannel(HitResult, TraceStart, TraceEnd, FRotator().Quaternion(), ECC_Visibility,
		FCollisionShape::MakeCapsule(CapsuleRadius - 1, CapsuleHalfHeight - 1), QueryParams);

	return !HitResult.bBlockingHit;
}

bool UParkourComponent::LedgeHang_MoveCornerTest(bool bRight)
{
	FHitResult HitResult;
	FVector TraceStart;
	FVector TraceEnd;

	FTransform HandsPos = LedgeHang_CornerMoveTestOffset * LedgeHang_GetCornerTransform();
	TraceStart = HandsPos.GetLocation();

	FVector RightVector = HandsPos.Rotator().RotateVector(FVector::ForwardVector);
	FVector Offset = RightVector * LedgeHang_MoveTestDistance * (bRight ? 1.0f : -1.0f);
	TraceEnd = TraceStart + Offset;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(ParentCharacter);

	GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, QueryParams);

	return !HitResult.bBlockingHit;
}

bool UParkourComponent::LedgeHang_MoveHoleTest(bool bRight)
{
	FHitResult HitResult;

	FVector TraceStart;
	FVector TraceEnd;

	FTransform HandsPos = LedgeHang_CornerMoveTestOffset * (LedgeHang_CornerOffset.Inverse() * ParentCharacter->GetActorTransform());
	FVector Offset = HandsPos.Rotator().RotateVector(FVector::RightVector);
	Offset *= (LedgeHang_MoveTestDistance * (bRight ? 1.0f : -1.0f));

	TraceStart = HandsPos.GetLocation() + Offset;
	TraceEnd = TraceStart - FVector(0.0f, 0.0f, LedgeHang_HoleTestDepth);

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(ParentCharacter);

	GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, QueryParams);

	return HitResult.bBlockingHit;
}

bool UParkourComponent::LedgeHang_UpCapsuleTrace(FVector GrabPoint)
{
	float CapsuleRadius = ParentCharacter->GetCapsuleComponent()->GetScaledCapsuleRadius();
	float CapsuleHalfHeight = ParentCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

	FHitResult HitResult;
	FVector TraceStart;
	FVector TraceEnd;

	TraceStart = ParentCharacter->GetActorLocation();
	TraceEnd = FVector(TraceStart.X, TraceStart.Y, GrabPoint.Z + CapsuleHalfHeight);

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(ParentCharacter);

	FCollisionShape CapsuleShape = FCollisionShape::MakeCapsule(CapsuleRadius - 1, CapsuleHalfHeight - 1);

	GetWorld()->SweepSingleByChannel(HitResult, TraceStart, TraceEnd, FRotator().Quaternion(), ECC_Visibility, CapsuleShape, QueryParams);

	if (HitResult.bBlockingHit)return false;

	TraceStart = TraceEnd;
	FVector CapsuleFwdVector = FVector(GrabPoint.X - TraceStart.X, GrabPoint.Y - TraceStart.Y, 0.0f);
	CapsuleFwdVector.Normalize();
	TraceEnd += (CapsuleFwdVector * LedgeHang_UpMinFloorLength);

	GetWorld()->SweepSingleByChannel(HitResult, TraceStart, TraceEnd, FRotator().Quaternion(), ECC_Visibility, CapsuleShape, QueryParams);

	return !HitResult.bBlockingHit;
}

bool UParkourComponent::LedgeHang_UpCrouchCapsuleTrace(FVector GrabPoint)
{
	float CapsuleRadius = ParentCharacter->GetCapsuleComponent()->GetScaledCapsuleRadius();

	float CapsuleHalfHeight_Walk = ParentCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	float CapsuleHalfHeight_Crouched = ParentCharacter->GetCharacterMovement()->GetCrouchedHalfHeight();

	float CapsuleHalfHeight_Actual = CapsuleHalfHeight_Crouched;

	FHitResult HitResult;
	FVector TraceStart;
	FVector TraceEnd;

	float CrouchedHalfHeightDiff = CapsuleHalfHeight_Walk - CapsuleHalfHeight_Crouched;
	TraceStart = ParentCharacter->GetActorLocation() - FVector(0.0f, 0.0f, CrouchedHalfHeightDiff);
	TraceEnd = FVector(TraceStart.X, TraceStart.Y, GrabPoint.Z + CapsuleHalfHeight_Actual);

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(ParentCharacter);

	FCollisionShape CapsuleShape = FCollisionShape::MakeCapsule(CapsuleRadius - 1, CapsuleHalfHeight_Actual - 1);

	GetWorld()->SweepSingleByChannel(HitResult, TraceStart, TraceEnd, FRotator().Quaternion(), ECC_Visibility, CapsuleShape, QueryParams);

	if (HitResult.bBlockingHit)return false;

	TraceStart = TraceEnd;
	FVector CapsuleFwdVector = FVector(GrabPoint.X - TraceStart.X, GrabPoint.Y - TraceStart.Y, 0.0f);
	CapsuleFwdVector.Normalize();
	TraceEnd += (CapsuleFwdVector * LedgeHang_UpMinFloorLength);

	GetWorld()->SweepSingleByChannel(HitResult, TraceStart, TraceEnd, FRotator().Quaternion(), ECC_Visibility, CapsuleShape, QueryParams);

	return !HitResult.bBlockingHit;
}

void UParkourComponent::UpdateLedgeMove()
{
	float CapsuleRadius = ParentCharacter->GetCapsuleComponent()->GetScaledCapsuleRadius();
	float CapsuleHalfHeight = ParentCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

	FHitResult HitResult;
	FVector TraceStart;
	FVector TraceEnd;

	TraceStart = ParentCharacter->GetActorLocation();
	TraceStart.Z += LedgeHang_WallTestZOffset;
	FVector Offset = ParentCharacter->GetActorForwardVector();
	Offset *= CapsuleRadius - LedgeHang_WallTestRadius + 30.0f;
	TraceEnd = TraceStart + Offset;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(ParentCharacter);
	
	GetWorld()->SweepSingleByChannel(HitResult, TraceStart, TraceEnd, FRotator().Quaternion(), ECC_Visibility,
		FCollisionShape::MakeSphere(LedgeHang_WallTestRadius), QueryParams);

	LedgeHang_bWall = HitResult.bBlockingHit;
}

FTransform UParkourComponent::LedgeHang_GetCornerTransform()
{
	return LedgeHang_CornerOffset.Inverse() * ParentCharacter->GetActorTransform();
}

void UParkourComponent::OnUpdateParkourMode_Event(EParkourMode NewMode, EParkourMode PrevMode)
{
	if (PrevMode == EParkourMode::EPM_LedgeHang)
	{
		LedgeHang_bMove = false;
		LedgeHang_bMoveRight = false;
	}
	if (PrevMode == EParkourMode::EPM_Ladder)
	{
		CurrentLadder = nullptr;
		Ladder_Step = -1;
	}

	if (NewMode == EParkourMode::EPM_None)
	{
		ParentCharacter->GetCapsuleComponent()->SetCapsuleRadius(DefaultCapsuleRadius);
		ParentCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
		ParentCharacter->GetCharacterMovement()->StopMovementImmediately();
	}
	if (NewMode == EParkourMode::EPM_LedgeHang)
	{
		ParentCharacter->GetCapsuleComponent()->SetCapsuleRadius(DefaultCapsuleRadius);
		ParentCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
		ParentCharacter->GetCharacterMovement()->StopMovementImmediately();
	}
	if (NewMode == EParkourMode::EPM_Ladder)
	{
		ParentCharacter->GetCapsuleComponent()->SetCapsuleRadius(DefaultCapsuleRadius);
		ParentCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
		ParentCharacter->GetCharacterMovement()->StopMovementImmediately();
	}
}

void UParkourComponent::SetParkourMode(EParkourMode Mode)
{
	if (Mode != ParkourMode)
	{
		EParkourMode PrevMode = ParkourMode;
		ParkourMode = Mode;
		OnUpdateParkourMode.Broadcast(ParkourMode, PrevMode);
	}
}

void UParkourComponent::MoveForward(float Scale)
{
	if (ParkourMode == EParkourMode::EPM_Ladder)
	{
		if (FMath::Abs(Scale) > 0.5f)
		{
			LadderMove(Scale > 0.0f);
		}
	}
}

void UParkourComponent::MoveRight(float Scale)
{
	LedgeHang_bMove = false;
	if (ParkourMode == EParkourMode::EPM_LedgeHang)
	{
		if (FMath::Abs(Scale) > 0.5f)
		{
			LedgeHangMove(Scale > 0.0f);
		}
	}
}

void UParkourComponent::CheckLedge()
{
	if (!ParentCharacter->GetCharacterMovement()->IsFalling())return;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(ParentCharacter);

	for (float Height : LedgeHang_TraceHandsHeightList)
	{
		FHitResult HitResult;

		FVector StartPoint = ParentCharacter->GetActorLocation() + FVector(0.0f, 0.0f, Height);
		FVector BackOffset = ParentCharacter->GetActorForwardVector() * LedgeHang_TraceBackLength * -1.0f;
		FVector FwdOffset = ParentCharacter->GetActorForwardVector() * LedgeHang_TraceLength;

		FVector TraceStart = StartPoint + BackOffset;
		FVector TraceEnd = StartPoint + FwdOffset;

		GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, QueryParams);

		/*if (HitResult.bBlockingHit)
		{
			DrawDebugLine(GetWorld(), TraceStart, HitResult.ImpactPoint, FColor(255, 0, 0), false, -1, 0, 0.5f);
			DrawDebugPoint(GetWorld(), HitResult.ImpactPoint, 2.0f, FColor(0, 255, 0), false, -1);
		}
		else
		{
			DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor(255, 0, 0), false, -1, 0, 0.5f);
		}*/

		if (HitResult.bBlockingHit)
		{
			FVector FloorLocation;
			if (LedgeHang_TraceFloor(HitResult.ImpactPoint, HitResult.ImpactNormal, FloorLocation))
			{
				FRotator LedgeRotation = (HitResult.ImpactNormal * -1).Rotation();
				FVector LedgeLocation = HitResult.ImpactPoint;
				LedgeLocation.Z = FloorLocation.Z;
				BeginLedgeHang(LedgeLocation, LedgeRotation);
				return;
			}
		}
	}
}

bool UParkourComponent::IsActiveParkour()
{
	return ParkourMode != EParkourMode::EPM_None;
}

void UParkourComponent::Jump()
{
	if (ParkourMode == EParkourMode::EPM_LedgeHang)
	{
		FRotator ControlRot;
		FRotator ActorRot;

		ControlRot = ParentCharacter->GetControlRotation();
		ControlRot = FRotator(0.0f, ControlRot.Yaw, 0.0f);
		ActorRot = ParentCharacter->GetActorRotation();
		ActorRot = FRotator(0.0f, ActorRot.Yaw, 0.0f);

		FVector ControlRotVector = ControlRot.RotateVector(FVector::ForwardVector);
		FVector ActorRotVector = ActorRot.RotateVector(FVector::ForwardVector);

		float Angle = (180.0) / UE_DOUBLE_PI * FMath::Acos(FVector::DotProduct(ControlRotVector, ActorRotVector));

		if (Angle > 90.0f)
		{
			LedgeHangJumpOff();
		}
		else
		{
			LedgeHangUp();
		}
	}
	if (ParkourMode == EParkourMode::EPM_Ladder)
	{
		LadderJumpOff();
	}
}

void UParkourComponent::Crouch()
{
	if (ParkourMode == EParkourMode::EPM_LedgeHang)
	{
		LedgeHangOff();
	}
	if (ParkourMode == EParkourMode::EPM_Ladder)
	{
		LadderOff();
	}
}

void UParkourComponent::Teleport(FTransform Target)
{
	ParentCharacter->SetActorLocation(Target.GetLocation(), false, nullptr, ETeleportType::TeleportPhysics);
	ParentCharacter->SetActorRotation(Target.Rotator(), ETeleportType::TeleportPhysics);
	ParentCharacter->GetCharacterMovement()->StopMovementImmediately();
}

void UParkourComponent::OnComponentActivated_Event(UActorComponent* Component, bool bReset)
{
	SetComponentTickEnabled(true);
}

void UParkourComponent::OnComponentDeactivated_Event(UActorComponent* Component)
{
	SetComponentTickEnabled(false);
	SetParkourMode(EParkourMode::EPM_None);
}

