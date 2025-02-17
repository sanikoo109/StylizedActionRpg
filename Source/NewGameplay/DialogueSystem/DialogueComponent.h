// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DialogueSystem/DialogueSystemTypes.h"
#include "DialogueComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDialogueComponent_OnUpdateCurrentPhraseSignature, FName, NewPhrase, FName, PrevPhrase);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDialogueComponent_OnSelectVariantSignature, ACharacter*, Char, FName, VariantName);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NEWGAMEPLAY_API UDialogueComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDialogueComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Dialogue Component")
	static void FindVariantData(FPhraseTableData Phrase, FName VariantName, bool& bValid, FPhraseVariant& Result);

	UPROPERTY(BlueprintReadOnly, Category = "Dialogue Component")
	UDialogueComponent* DialogueTarget;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Dialogue Component")
	FText Name;

	UPROPERTY(BlueprintReadOnly, Category = "Dialogue Component")
	FName CurrentPhrase;

	UPROPERTY(BlueprintReadOnly, Category = "Dialogue Component")
	int32 SelectedVariantID;

	UPROPERTY(EditAnywhere, Category = "Dialogue Component")
	UDataTable* PhrasesTable;

	UFUNCTION(BlueprintCallable, Category = "Dialogue Component")
	void SetCurrentPhrase(FName Phrase);

	UFUNCTION(BlueprintCallable, Category = "Dialogue Component")
	void NextPhrase();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Dialogue Component")
	bool IsActivePhrase();

	UFUNCTION(BlueprintCallable, Category = "Dialogue Component")
	void SelectVariant(int32 ID);

	UFUNCTION(BlueprintCallable, Category = "Dialogue Component")
	void SetSelectedVariant(int32 VariantID);

	UFUNCTION(BlueprintCallable, Category = "Dialogue Component")
	void OffsetVariant(int32 Value);

	UFUNCTION()
	void OnUpdateCurrentPhrase_Event(FName NewPhrase, FName PrevPhrase);

	UFUNCTION(BlueprintCallable, Category = "Dialogue Component")
	void BeginDialogue(FName StartPhrase, UDialogueComponent* TargetDialogueComponent);

	UPROPERTY(BlueprintAssignable, Category = "Dialogue Component")
	FDialogueComponent_OnUpdateCurrentPhraseSignature OnUpdateCurrentPhrase;

	UPROPERTY(BlueprintAssignable, Category = "Dialogue Component")
	FDialogueComponent_OnSelectVariantSignature OnSelectVariant;

protected:

	UPROPERTY()
	class ACharacter* OwnerCharacter;

};
