// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DialogueSystemTypes.generated.h"

USTRUCT(BlueprintType)
struct FPhraseVariant
{

	GENERATED_BODY()

public:

	FPhraseVariant() {};

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Dialogue Phrase Variant")
	FName Name;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Dialogue Phrase Variant")
	FText Text;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Dialogue Phrase Variant")
	FName NextPhrase;

};

USTRUCT(BlueprintType)
struct FPhraseTableData : public FTableRowBase
{

	GENERATED_BODY()

public:

	FPhraseTableData() {};

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Dialogue Phrase")
	FText Speaker;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Dialogue Phrase")
	FText Text;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Dialogue Phrase")
	TArray<FPhraseVariant> Variants;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Dialogue Phrase")
	FName NextPhrase;

};
