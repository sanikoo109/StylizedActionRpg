// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatSystem/CombatSystemTypes.h"
#include "SpellSystem/SpellSystemTypes.h"
#include "CombatComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FCombatComponent_OnDamageTargetSignature, AActor*, Target, FGameDamage, AppliedDamage, bool, bRightArm, bool, bKill);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCombatComponent_OnUpdateAttackingSignature, bool, IsAttacking);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCombatComponent_OnUpdateBlockingSignature, bool, IsBlocking);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FCombatComponent_OnCastSpellSignature, FName, Spell, FTransform, SpawnTransform, FTransform, PreviewTransform);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCombatComponent_OnUpdateShowAmmoSignature, bool, IsShow);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FCombatComponent_OnUpdateCurrentMoveSignature, FName, NewMove, FName, PrevMove);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCombatComponent_OnUpdateShowReloadAmmoSignature, bool, bShow);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCombatComponent_OnUpdateReloadingSignature, bool, IsReloading);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class NEWGAMEPLAY_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCombatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	
	UFUNCTION(BlueprintCallable, Category = "Combat Component")
	void AttackPress();
	
	UFUNCTION(BlueprintCallable, Category = "Combat Component")
	void AttackRelease();
	
	UFUNCTION(BlueprintCallable, Category = "Combat Component")
	void BlockPress();
	
	UFUNCTION(BlueprintCallable, Category = "Combat Component")
	void BlockRelease();
	
	UFUNCTION(BlueprintCallable, Category = "Combat Component")
	void Attack(FName Spell);

	UFUNCTION()
	void AttackSelectMove(FName Spell, FName& OutMove, bool& CanInAir);

	UFUNCTION(BlueprintImplementableEvent, Category = "Combat Component")
	bool GetSpellMove(FName Spell, FName& OutMove);

	UFUNCTION()
	void AttackNew(FName Spell);

	UFUNCTION()
	void AttackContinue(FName Spell, FName SelectedMove, int32 Section);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Combat Component")
	void AttackEvent(UAnimMontage* MontageToPlay, float PlayRate);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Combat Component")
	void ItemAttackEvent(UAnimMontage* MontageToPlay, float PlayRate);

	UFUNCTION(BlueprintCallable, Category = "Combat Component")
	void AttackEndAnim();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Combat Component")
	void AttackCast();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Combat Component")
	void AttackShoot();

	UFUNCTION(BlueprintCallable, Category = "Combat Component")
	void SetAttackTraceActive(bool bRight, bool IsActive);

	UFUNCTION(BlueprintCallable, Category = "Combat Component")
	void SetShowAmmo(bool bShow);
	
	UFUNCTION(BlueprintImplementableEvent, Category = "Combat Component")
	void ShowTrail_L();

	UFUNCTION(BlueprintImplementableEvent, Category = "Combat Component")
	void ShowTrail_R();

	UFUNCTION(BlueprintImplementableEvent, Category = "Combat Component")
	void AttackRelease_Begin(int32 ID);

	UFUNCTION(BlueprintImplementableEvent, Category = "Combat Component")
	void AttackRelease_Action(int32 ID);

	UFUNCTION()
	void Trace(bool bRight);

	UFUNCTION(BlueprintImplementableEvent, Category = "Combat Component")
	FVector GetWeaponTracePoint(bool bEnd);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, BlueprintPure, Category = "Combat Component")
	AInventoryItem* GetWeaponItem();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, BlueprintPure, Category = "Combat Component")
	USkeletalMeshComponent* GetWeaponMesh();
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Combat Component")
	UAnimInstance* GetAnimInstance();
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Combat Component")
	bool IsCharPlayingMontage();
	
	UFUNCTION(BlueprintCallable, Category = "Combat Component")
	void GetAttackSection(FString& Prefix, int32& ID, bool& bValid);
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Combat Component")
	USkeletalMeshComponent* GetCharMesh();
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Combat Component")
	FVector GetSocketLocation_Safe(USceneComponent* Comp, FName Socket);
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Combat Component")
	FRotator GetSocketRotation_Safe(USceneComponent* Comp, FName Socket);
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Combat Component")
	FTransform GetSocketTransform_Safe(USceneComponent* Comp, FName Socket);
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Combat Component")
	FGameDamage GetDamage();
	
	UFUNCTION(BlueprintCallable, Category = "Combat Component")
	void CheckBlocking();

	UFUNCTION(BlueprintImplementableEvent, Category = "Combat Component")
	void CheckBlocking_NextFrame();
	
	UFUNCTION()
	void OnUpdateAttacking_Event(bool IsAttacking);

	UFUNCTION()
	void OnMontageStarted_Event(UAnimMontage* Montage);

	UFUNCTION()
	void OnMontageEnded_Event(UAnimMontage* Montage, bool bInterrupted);
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Combat Component")
	bool IsParrying();
	
	UFUNCTION()
	bool IsParryCooldown();
	
	UFUNCTION(BlueprintCallable, BlueprintPure, BlueprintImplementableEvent, Category = "Combat Component")
	class AInventoryItem* GetCurrentWeapon();
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, BlueprintPure, Category = "Combat Component")
	AActor* GetCurrentWeaponCauser();
	
	UFUNCTION(BlueprintCallable, Category = "Combat Component")
	void CheckCombatAttackData();
	
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Combat Component")
	void Reload();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Combat Component")
	void PlayReloadAnim_Char(UAnimMontage* MontageToPlay, float PlayRate);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Combat Component")
	void PlayReloadAnim_Item(UAnimMontage* MontageToPlay, float PlayRate);

	UFUNCTION(BlueprintCallable, Category = "Combat Component")
	void ReloadEnd();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Combat Component")
	void ReloadExecute();
	
	UFUNCTION(BlueprintCallable, Category = "Combat Component")
	void SetShowReloadAmmo(bool bShow);
	
	UFUNCTION(BlueprintCallable, Category = "Combat Component")
	void SetReloading(bool Value);

public:
	
	UPROPERTY(BlueprintReadOnly, Category = "Combat Component")
	bool bAttackPressed;
	
	UPROPERTY(BlueprintReadOnly, Category = "Combat Component")
	bool bBlockPressed;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Combat Component")
	FName CurrentMove;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Combat Component")
	TMap<ESpellCastAnimType, FName> SpellCastMove;
	
	UPROPERTY(BlueprintReadOnly, Category = "Combat Component")
	class AMyCharacter* ParentChar;

	UPROPERTY(EditAnywhere, Category = "Combat Component")
	TSubclassOf<class UDamageType> PointDamageType;

	UPROPERTY(EditAnywhere, Category = "Combat Component")
	UDataTable* CombatMoveTable;
	
	UPROPERTY(BlueprintReadWrite, Category = "Combat Component")
	FCombatAttackData CombatAttackData;
	
	UPROPERTY()
	FCombatAttackData Cached_CombatAttackData;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Combat Component")
	FGameDamage AttackDamage;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Combat Component")
	float AttackSpeed = 1.0f;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Combat Component")
	float CastSpeed = 1.0f;
	
	UPROPERTY(BlueprintReadWrite, Category = "Combat Component")
	bool TraceHavePrev;
	
	UPROPERTY()
	FVector Prev_TraceStart;
	
	UPROPERTY()
	FVector Prev_TraceEnd;
	
	UPROPERTY(EditAnywhere, Category = "Combat Component")
	int32 TraceSubstepping = 8;
	
	UPROPERTY(BlueprintReadWrite, Category = "Combat Component")
	bool bAttacking;
	
	UPROPERTY(BlueprintReadOnly, Category = "Combat Component")
	bool bBlocking;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Combat Component")
	float ParryWindow = 0.25f;
	
	UPROPERTY(BlueprintReadOnly, Category = "Combat Component")
	float ParryRemaining;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Combat Component")
	float ParryCooldown = 1.0f;
	
	UPROPERTY(BlueprintReadOnly, Category = "Combat Component")
	float ParryCooldownRemaining;
	
	UPROPERTY(BlueprintReadOnly, Category = "Combat Component")
	bool bReloading;
	
	UPROPERTY(BlueprintReadOnly, Category = "Combat Component")
	bool bShowReloadAmmo;

	UPROPERTY(BlueprintAssignable, Category = "Combat Component")
	FCombatComponent_OnDamageTargetSignature OnDamageTarget;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Combat Component")
	FCombatComponent_OnUpdateAttackingSignature OnUpdateAttacking;

	UPROPERTY(BlueprintAssignable, Category = "Combat Component")
	FCombatComponent_OnUpdateBlockingSignature OnUpdateBlocking;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Combat Component")
	FCombatComponent_OnCastSpellSignature OnCastSpell;

	UPROPERTY(BlueprintAssignable, Category = "Combat Component")
	FCombatComponent_OnUpdateShowAmmoSignature OnUpdateShowAmmo;

	UPROPERTY(BlueprintAssignable, Category = "Combat Component")
	FCombatComponent_OnUpdateCurrentMoveSignature OnUpdateCurrentMove;

	UPROPERTY(BlueprintAssignable, Category = "Combat Component")
	FCombatComponent_OnUpdateShowReloadAmmoSignature OnUpdateShowReloadAmmo;
	
	UPROPERTY(BlueprintAssignable, Category = "Combat Component")
	FCombatComponent_OnUpdateReloadingSignature OnUpdateReloading;

};
