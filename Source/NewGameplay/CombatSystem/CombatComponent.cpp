// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatSystem/CombatComponent.h"
#include "MyCharacter.h"
#include "NewGameplay.h"
#include "Kismet/GameplayStatics.h"
#include "InventorySystem/InventoryItem.h"
#include "CombatSystem/GameDamageInterface.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values for this component's properties
UCombatComponent::UCombatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	OnUpdateAttacking.AddDynamic(this, &UCombatComponent::OnUpdateAttacking_Event);
	ParentChar = Cast<AMyCharacter>(GetOwner());

	ParentChar->GetMesh()->GetAnimInstance()->OnMontageStarted.AddDynamic(this, &UCombatComponent::OnMontageStarted_Event);
	ParentChar->GetMesh()->GetAnimInstance()->OnMontageEnded.AddDynamic(this, &UCombatComponent::OnMontageEnded_Event);
	
}


// Called every frame
void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (IsActive())
	{
		if (CombatAttackData.bTraceL)
		{
			Trace(false);
		}
		if (CombatAttackData.bTraceR)
		{
			Trace(true);
		}
		if (CombatAttackData.bDeferred)
		{
			Attack(CombatAttackData.DeferredSpell);
		}
		if (ParryRemaining > 0.0f)
		{
			float TargetValue = ParryRemaining - DeltaTime;
			ParryRemaining = FMath::Clamp(TargetValue, 0.0f, TargetValue);
			if (ParryRemaining <= 0.0f)
			{
				CheckBlocking();
			}
		}
		if (ParryCooldownRemaining > 0.0f)
		{
			float TargetValue = ParryCooldownRemaining - DeltaTime;
			ParryCooldownRemaining = FMath::Clamp(TargetValue, 0.0f, TargetValue);
		}
	}
}

void UCombatComponent::AttackPress()
{
	Attack("None");
}

void UCombatComponent::AttackRelease()
{
	if (FCombatMoveTableData* MoveRow = CombatMoveTable->FindRow<FCombatMoveTableData>(CombatAttackData.CurrentMove, "Cant find combat move row"))
	{
		if (MoveRow->bCanReleaseAction)
		{
			FString OutPrefix;
			int32 OutID;
			bool OutValid;
			GetAttackSection(OutPrefix, OutID, OutValid);
			if (OutPrefix == "Begin")
			{
				AttackRelease_Begin(OutID);
			}
			if (OutPrefix == "Action")
			{
				AttackRelease_Action(OutID);
			}
			if (OutPrefix == "ActionLoop")
			{
				AttackRelease_Action(OutID);
			}
		}
	}
}

void UCombatComponent::BlockPress()
{
	if (IsParryCooldown())return;

	bBlockPressed = true;
	CheckBlocking();
	if (bBlocking)
	{
		ParryRemaining = ParryWindow;
		ParryCooldownRemaining = ParryCooldown;
	}
}

void UCombatComponent::BlockRelease()
{
	bBlockPressed = false;
	CheckBlocking();
}

void UCombatComponent::Attack(FName Spell)
{
	if (!IsActive())return;

	if (AInventoryItem* CurrentWeapon = GetWeaponItem())
	{
		if (!CurrentWeapon->CanAttack())return;
	}

	FString OutPrefix;
	int32 OutID;
	bool OutValid;
	GetAttackSection(OutPrefix, OutID, OutValid);

	FName SelectedMove;
	bool bCanInAir;

	if (OutPrefix == "Action")
	{
		AttackSelectMove(Spell, SelectedMove, bCanInAir);

		CombatAttackData.bDeferred = true;
		CombatAttackData.DeferredSpell = Spell;
		CombatAttackData.DeferredMove = SelectedMove;

		CheckCombatAttackData();
	}
	else if (OutPrefix == "End")
	{
		AttackSelectMove(Spell, SelectedMove, bCanInAir);

		CombatAttackData.bDeferred = false;
		CombatAttackData.SpellToCast = Spell;
		CombatAttackData.DeferredSpell = "";
		CombatAttackData.DeferredMove = "";

		if (!bCanInAir && ParentChar->GetCharacterMovement()->IsFalling())return;
		if (CombatAttackData.CurrentMove == SelectedMove)
		{
			AttackContinue(Spell, SelectedMove, OutID);
		}
		else
		{
			AttackNew(Spell);
		}
	}
	else
	{
		if (GetAnimInstance()->Montage_IsPlaying(nullptr))return;
		AttackSelectMove(Spell, SelectedMove, bCanInAir);
		if (!bCanInAir && ParentChar->GetCharacterMovement()->IsFalling())return;

		CombatAttackData = FCombatAttackData();
		CheckCombatAttackData();

		AttackNew(Spell);
	}

}

void UCombatComponent::AttackSelectMove(FName Spell, FName& OutMove, bool& CanInAir)
{
	OutMove = CurrentMove;

	FName FindedSpellMove;
	if (GetSpellMove(Spell, FindedSpellMove))
	{
		OutMove = FindedSpellMove;
	}

	if (FCombatMoveTableData* CombatMoveRow = CombatMoveTable->FindRow<FCombatMoveTableData>(OutMove, "Cant find combat move in table"))
	{
		CanInAir = CombatMoveRow->bCanInAir;
	}
}

void UCombatComponent::AttackNew(FName Spell)
{
	FName SelectedMove;
	bool bCanInAir;
	AttackSelectMove(Spell, SelectedMove, bCanInAir);

	FCombatMoveTableData* CombatMoveRow = CombatMoveTable->FindRow<FCombatMoveTableData>(SelectedMove, "Cant find combat move in table");
	if (!CombatMoveRow)return;

	CombatAttackData.MoveCount = CombatMoveRow->MoveCount;
	CombatAttackData.CurrentMontage = CombatMoveRow->Montage;
	CombatAttackData.SpellToCast = Spell;
	CombatAttackData.CurrentMove = SelectedMove;
	CombatAttackData.bShowAmmo = false;

	CheckCombatAttackData();
	float PlayRate = CombatMoveRow->PlayRate;
	PlayRate *= ((Spell == "") ? AttackSpeed : CastSpeed);

	AttackEvent(CombatMoveRow->Montage, PlayRate);
	ItemAttackEvent(CombatMoveRow->Montage, PlayRate);
	bAttacking = true;
	OnUpdateAttacking.Broadcast(bAttacking);

}

void UCombatComponent::AttackContinue(FName Spell, FName SelectedMove, int32 Section)
{
	int32 NextSectionID = ((Section + 1) % CombatAttackData.MoveCount);
	FName NextSection = FName("Begin_" + FString::FromInt(NextSectionID));

	GetAnimInstance()->Montage_JumpToSection(NextSection, CombatAttackData.CurrentMontage);

	if (USkeletalMeshComponent* WeaponMesh = GetWeaponMesh())
	{
		if (UAnimInstance* WeaponAnimInstance = WeaponMesh->GetAnimInstance())
		{
			WeaponAnimInstance->Montage_JumpToSection(NextSection, nullptr);
		}
	}

	FCombatMoveTableData* CombatMoveRow = CombatMoveTable->FindRow<FCombatMoveTableData>(CombatAttackData.CurrentMove, "Cant find combat move in table");
	if (!CombatMoveRow)return;

	float PlayRate = CombatMoveRow->PlayRate;
	PlayRate *= ((Spell == "") ? AttackSpeed : CastSpeed);

	GetAnimInstance()->Montage_SetPlayRate(CombatAttackData.CurrentMontage, PlayRate);

	CombatAttackData.DamageRegister.Empty();
	CombatAttackData.bShowAmmo = false;
	CheckCombatAttackData();
}

void UCombatComponent::AttackEndAnim()
{
	bAttacking = false;
	OnUpdateAttacking.Broadcast(bAttacking);
	CombatAttackData = FCombatAttackData();
	CheckCombatAttackData();
}

void UCombatComponent::SetAttackTraceActive(bool bRight, bool IsActive)
{

	if (bRight)CombatAttackData.bTraceR = IsActive;
	if (!bRight)CombatAttackData.bTraceL = IsActive;

	if (IsActive)TraceHavePrev = false;
	CheckCombatAttackData();
}

void UCombatComponent::SetShowAmmo(bool bShow)
{
	CombatAttackData.bShowAmmo = true;
	CheckCombatAttackData();
}

void UCombatComponent::Trace(bool bRight)
{
	if (bRight)
	{
		ShowTrail_R();
	}
	else
	{
		ShowTrail_L();
	}

	TArray<FVector> TraceList_Start;
	TArray<FVector> TraceList_End;

	if (!TraceHavePrev)
	{
		TraceList_Start.Add(GetWeaponTracePoint(false));
		TraceList_End.Add(GetWeaponTracePoint(true));
	}
	if (TraceHavePrev) //Substepping
	{
		for (int i = 0; i < TraceSubstepping; ++i)
		{
			float fID = i;
			float fMaxSubsteps = TraceSubstepping;
			float Alpha = fID / fMaxSubsteps;

			TraceList_Start.Add(FMath::Lerp(Prev_TraceStart, GetWeaponTracePoint(false), Alpha));
			TraceList_End.Add(FMath::Lerp(Prev_TraceEnd, GetWeaponTracePoint(true), Alpha));
		}
	}

	FVector TraceStart = FVector();
	FVector TraceEnd = FVector();

	for (int32 i = 0; i < TraceList_Start.Num(); ++i)
	{
		TraceStart = TraceList_Start[i];
		TraceEnd = TraceList_End[i];

		FHitResult HitResult;

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(ParentChar);

		GetWorld()->SweepSingleByChannel(HitResult, TraceStart, TraceEnd, FRotator().Quaternion(), ECC_Damage, FCollisionShape::MakeSphere(10.0f), QueryParams);

		/*if (HitResult.bBlockingHit)
		{
			DrawDebugLine(GetWorld(), TraceStart, HitResult.ImpactPoint, FColor(255, 0, 0), false, 5.0f, 0, 0.5f);
			DrawDebugPoint(GetWorld(), HitResult.ImpactPoint, 2.0f, FColor(0, 255, 0), false, 5.0f);
		}
		else
		{
			DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor(255, 0, 0), false, 5.0f, 0, 0.5f);
		}*/

		if (HitResult.bBlockingHit)
		{
			if (!CombatAttackData.DamageRegister.Contains(HitResult.GetActor()))
			{
				CheckCombatAttackData();
				CombatAttackData.DamageRegister.Add(HitResult.GetActor());
				AMyCharacter* Char = Cast<AMyCharacter>(HitResult.GetActor());
				if (Char)
				{
					if (Char->bEnemy == ParentChar->bEnemy)return;
				}

				FVector HitFromDirection = ParentChar->GetActorLocation() - HitResult.GetActor()->GetActorLocation();
				HitFromDirection.Normalize();
				FGameDamage AppliedDamage;
				bool IsKill = false;

				if (HitResult.GetActor()->Implements<UGameDamageInterface>())
				{
					IGameDamageInterface::Execute_ApplyGameDamage(HitResult.GetActor(), GetDamage(), ParentChar->GetController(), GetCurrentWeaponCauser(), true, HitFromDirection, HitResult, AppliedDamage);
					if (Char)
					{
						IsKill = Char->IsDead();
					}
				}
				else
				{
					UGameplayStatics::ApplyPointDamage(HitResult.GetActor(), GetDamage().GetSum(), HitFromDirection, HitResult, ParentChar->GetController(), GetCurrentWeaponCauser(), PointDamageType);
					AppliedDamage = GetDamage();
				}

				OnDamageTarget.Broadcast(HitResult.GetActor(), AppliedDamage, true, IsKill);
			}
		}
	}

	TraceHavePrev = true;
	Prev_TraceStart = TraceStart;
	Prev_TraceEnd = TraceEnd;
}

UAnimInstance* UCombatComponent::GetAnimInstance()
{
	if (USkeletalMeshComponent* CharMesh = GetCharMesh())
	{
		return CharMesh->GetAnimInstance();
	}
	return nullptr;
}

bool UCombatComponent::IsCharPlayingMontage()
{
	if (UAnimInstance* AnimInstance = GetAnimInstance())
	{
		return AnimInstance->IsAnyMontagePlaying();
	}
	return false;
}

void UCombatComponent::GetAttackSection(FString& Prefix, int32& ID, bool& bValid)
{
	Prefix = "";
	ID = 0;
	bValid = false;

	if (IsCharPlayingMontage())
	{
		FString SectionString = GetAnimInstance()->Montage_GetCurrentSection(CombatAttackData.CurrentMontage).ToString();
		FString IndexString;
		bValid = SectionString.Split("_", &Prefix, &IndexString);
		ID = FCString::Atoi(*IndexString);
	}
}

USkeletalMeshComponent* UCombatComponent::GetCharMesh()
{
	if (!ParentChar)return nullptr;
	return ParentChar->GetMesh();
}

FVector UCombatComponent::GetSocketLocation_Safe(USceneComponent* Comp, FName Socket)
{
	if (!Comp)return FVector();
	return Comp->GetSocketLocation(Socket);
}

FRotator UCombatComponent::GetSocketRotation_Safe(USceneComponent* Comp, FName Socket)
{
	if (!Comp)return FRotator();
	return Comp->GetSocketRotation(Socket);
}

FTransform UCombatComponent::GetSocketTransform_Safe(USceneComponent* Comp, FName Socket)
{
	if (!Comp)return FTransform();
	return Comp->GetSocketTransform(Socket, ERelativeTransformSpace::RTS_World);
}

FGameDamage UCombatComponent::GetDamage()
{
	if (AInventoryItem* WeaponRef = GetCurrentWeapon())
	{
		return WeaponRef->GetDamage();
	}
	return AttackDamage;
}

void UCombatComponent::CheckBlocking()
{
	bool Local_TargetBlocking = !bAttacking && !IsCharPlayingMontage() && (bBlockPressed || IsParrying());

	if (Local_TargetBlocking != bBlocking)
	{
		bBlocking = Local_TargetBlocking;
		OnUpdateBlocking.Broadcast(bBlocking);
	}
}

void UCombatComponent::OnUpdateAttacking_Event(bool IsAttacking)
{
	CheckBlocking();
}

void UCombatComponent::OnMontageStarted_Event(UAnimMontage* Montage)
{
	CheckBlocking();
}

void UCombatComponent::OnMontageEnded_Event(UAnimMontage* Montage, bool bInterrupted)
{
	CheckBlocking_NextFrame();
}

bool UCombatComponent::IsParrying()
{
	return ParryRemaining > 0.0f;
}

bool UCombatComponent::IsParryCooldown()
{
	return ParryCooldownRemaining > 0.0f;
}

void UCombatComponent::CheckCombatAttackData()
{
	if (CombatAttackData.bShowAmmo != Cached_CombatAttackData.bShowAmmo)
	{
		OnUpdateShowAmmo.Broadcast(CombatAttackData.bShowAmmo);
	}
	if (CombatAttackData.CurrentMove != Cached_CombatAttackData.CurrentMove)
	{
		OnUpdateCurrentMove.Broadcast(CombatAttackData.CurrentMove, Cached_CombatAttackData.CurrentMove);
	}
	Cached_CombatAttackData = CombatAttackData;
}

void UCombatComponent::ReloadEnd()
{
	SetReloading(false);
	SetShowReloadAmmo(false);
}

void UCombatComponent::SetShowReloadAmmo(bool bShow)
{
	if (bShow != bShowReloadAmmo)
	{
		bShowReloadAmmo = bShow;
		OnUpdateShowReloadAmmo.Broadcast(bShowReloadAmmo);
	}
}

void UCombatComponent::SetReloading(bool Value)
{
	if (Value != bReloading)
	{
		bReloading = Value;
		OnUpdateReloading.Broadcast(bReloading);
	}
}
