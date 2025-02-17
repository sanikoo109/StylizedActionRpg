// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ParkourComponent.generated.h"

UENUM(BlueprintType)
enum class EParkourMode : uint8
{

	EPM_None UMETA(DisplayName = "None"),
	EPM_LedgeHang UMETA(DisplayName = "Ledge Hang"),
	EPM_LedgeStand UMETA(DisplayName = "Ledge Stand"),
	EPM_Ladder UMETA(DisplayName = "Ladder"),

};

class ALadder;
class AMyCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FParkourComponent_OnUpdateParkourModeSignature, EParkourMode, NewMode, EParkourMode, PrevMode);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class NEWGAMEPLAY_API UParkourComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UParkourComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//PARKOUR GENERAL

	UPROPERTY(BlueprintReadOnly, Category = "Parkour")
	AMyCharacter* ParentCharacter;

	UPROPERTY(BlueprintReadOnly, Category = "Parkour")
	EParkourMode ParkourMode;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Parkour")
	float DefaultCapsuleRadius = 25.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Parkour")
	bool bUpdateHeight;

	UFUNCTION(BlueprintCallable, Category = "Parkour")
	void SetUpdatingHeight(bool Value);

	UPROPERTY(BlueprintReadOnly, Category = "Parkour")
	float CapsuleDefaultHalfHeight;

	//LADDER

	UPROPERTY(BlueprintReadOnly, Category = "Ladder")
	ALadder* CurrentLadder;

	UPROPERTY(BlueprintReadOnly, Category = "Ladder")
	int32 Ladder_Step = -1;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ladder")
	float Ladder_CapsuleRadius = 25.0f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ladder")
	FTransform Ladder_Offset;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ladder")
	int32 Ladder_StepHeight = 4;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ladder")
	float Ladder_JumpOffXYPower = 500.0f;

	UFUNCTION(BlueprintCallable, Category = "Ladder")
	void BeginLadder(ALadder* LadderRef, int32 Step);

	UFUNCTION(BlueprintImplementableEvent, Category = "Ladder")
	void BeginLadder_Event();

	UFUNCTION(BlueprintCallable, Category = "Ladder")
	void LadderAnimEnded();

	UFUNCTION(BlueprintCallable, Category = "Ladder")
	void UpdateLadderPos();

	UFUNCTION(BlueprintCallable, Category = "Ladder")
	void LadderMove(bool bUp);

	UFUNCTION(BlueprintImplementableEvent, Category = "Ladder")
	void Ladder_ExitDown();

	UFUNCTION(BlueprintImplementableEvent, Category = "Ladder")
	void Ladder_ExitUp();

	UFUNCTION(BlueprintImplementableEvent, Category = "Ladder")
	void Ladder_PlayStepAnim(bool bUp);

	UFUNCTION(BlueprintImplementableEvent, Category = "Ladder")
	void Ladder_StopMontage();

	UFUNCTION(BlueprintCallable, BlueprintPure, BlueprintImplementableEvent, Category = "Ladder")
	bool IsLadderBusy();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Ladder")
	int32 ClampLadderStep(ALadder* Ladder, int32 InStep);

	UFUNCTION(BlueprintCallable, Category = "Ladder")
	void LadderJumpOff();

	UFUNCTION(BlueprintCallable, Category = "Ladder")
	void LadderOff();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Ladder")
	int32 GetMaxLadderStep(ALadder* Ladder);

	//LEDGE HANG

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ledge Hang")
	float LedgeHang_TraceHandsHeight = 60.0f;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ledge Hang")
	TArray<float> LedgeHang_TraceHandsHeightList;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ledge Hang")
	float LedgeHang_TraceBackLength = 50.0f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ledge Hang")
	float LedgeHang_TraceLength = 100.0f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ledge Hang")
	float LedgeHang_TraceDepth = 10.0f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ledge Hang")
	float LedgeHang_TraceHeight = 160.0f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ledge Hang")
	float LedgeHang_HandsRadius = 10.0f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ledge Hang")
	FTransform LedgeHang_CornerOffset;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ledge Hang")
	float LedgeHang_MoveTestDistance = 25.0f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ledge Hang")
	FTransform LedgeHang_CornerMoveTestOffset;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ledge Hang")
	float LedgeHang_HoleTestDepth = 25.0f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ledge Hang")
	float LedgeHang_UpMinFloorLength = 25.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Ledge Hang")
	bool LedgeHang_bMove;

	UPROPERTY(BlueprintReadOnly, Category = "Ledge Hang")
	bool LedgeHang_bMoveRight;

	UPROPERTY(BlueprintReadOnly, Category = "Ledge Hang")
	bool LedgeHang_bWall;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ledge Hang")
	float LedgeHang_WallTestRadius = 25.0f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ledge Hang")
	float LedgeHang_WallTestZOffset = -45.0f;

	UFUNCTION(BlueprintCallable, Category = "Ladder")
	void BeginLedgeHang(FVector CornerLocation, FRotator CornerRotation);

	UFUNCTION()
	bool LedgeHang_TraceFloor(FVector ImpactPoint, FVector ImpactNormal, FVector& FloorLocation);

	UFUNCTION()
	bool LedgeHang_CapsuleTrace(FVector CornerPoint, FVector CornerNormal);

	UFUNCTION()
	bool LedgeHang_HandTrace(FVector GrabPoint, FVector CornerPoint, FVector CornerNormal);

	UFUNCTION(BlueprintCallable, Category = "Ladder")
	void LedgeHangOff();

	UFUNCTION(BlueprintCallable, Category = "Ladder")
	void LedgeHangJumpOff();

	UFUNCTION(BlueprintCallable, Category = "Ladder")
	void LedgeHangMove(bool bRight);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Ladder")
	bool IsLedgeHangBusy();

	UFUNCTION(BlueprintCallable, Category = "Ladder")
	void LedgeHangUp();

	UFUNCTION(BlueprintImplementableEvent, Category = "Ladder")
	bool IsPlayingLedgeHangUpAnim();

	UFUNCTION(BlueprintImplementableEvent, Category = "Ladder")
	void PlayMontage_LedgeHangUp();

	UFUNCTION(BlueprintCallable, Category = "Ladder")
	void EndMontage_LedgeHangUp();

	UFUNCTION(BlueprintImplementableEvent, Category = "Ladder")
	void PlayMontage_LedgeHangUpCrouch();

	UFUNCTION(BlueprintCallable, Category = "Ladder")
	void EndMontage_LedgeHangUpCrouch();

	UFUNCTION(BlueprintCallable, Category = "Ladder")
	void LedgeHangUpCrouch_UpdateState(int32 Frame);

	UFUNCTION()
	bool LedgeHang_MoveTest(bool bRight);

	UFUNCTION()
	bool LedgeHang_MoveCapsuleTest(bool bRight);

	UFUNCTION()
	bool LedgeHang_MoveCornerTest(bool bRight);

	UFUNCTION()
	bool LedgeHang_MoveHoleTest(bool bRight);

	UFUNCTION()
	bool LedgeHang_UpCapsuleTrace(FVector GrabPoint);

	UFUNCTION()
	bool LedgeHang_UpCrouchCapsuleTrace(FVector GrabPoint);

	UFUNCTION()
	void UpdateLedgeMove();

	UFUNCTION()
	FTransform LedgeHang_GetCornerTransform();

	//PARKOUR

	UFUNCTION()
	void OnUpdateParkourMode_Event(EParkourMode NewMode, EParkourMode PrevMode);

	UFUNCTION()
	void SetParkourMode(EParkourMode Mode);

	UFUNCTION(BlueprintCallable, Category = "Parkour")
	void MoveForward(float Scale);

	UFUNCTION(BlueprintCallable, Category = "Parkour")
	void MoveRight(float Scale);

	UFUNCTION()
	void CheckLedge();

	UFUNCTION(BlueprintImplementableEvent, Category = "Parkour")
	void UpdateHeightByAnimCurve();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Parkour")
	bool IsActiveParkour();

	UFUNCTION(BlueprintCallable, Category = "Parkour")
	void Jump();

	UFUNCTION(BlueprintCallable, Category = "Parkour")
	void Crouch();

	UFUNCTION()
	void Teleport(FTransform Target);

	UPROPERTY(BlueprintAssignable, Category = "Parkour")
	FParkourComponent_OnUpdateParkourModeSignature OnUpdateParkourMode;

	UFUNCTION()
	void OnComponentActivated_Event(UActorComponent* Component, bool bReset);

	UFUNCTION()
	void OnComponentDeactivated_Event(UActorComponent* Component);

};
