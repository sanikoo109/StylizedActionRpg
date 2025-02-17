// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractInterface.generated.h"

class ACharacter;

UENUM(BlueprintType)
enum class EInteractType : uint8
{

	EIT_None UMETA(DisplayName = "None"),
	EIT_Default UMETA(DisplayName = "Default"),
	EIT_Fail UMETA(DisplayName = "Fail")

};

USTRUCT(BlueprintType)
struct FInteractInfo
{

	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Interact Info")
	EInteractType Type;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Interact Info")
	FText Text;

};

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class NEWGAMEPLAY_API IInteractInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintNativeEvent, Category = "Interact Interface")
	void Interact(ACharacter* Char, UPrimitiveComponent* TargetComp);

	UFUNCTION(BlueprintNativeEvent, Category = "Interact Interface")
	FInteractInfo GetInteractInfo(ACharacter* Char, UPrimitiveComponent* TargetComp);

};
