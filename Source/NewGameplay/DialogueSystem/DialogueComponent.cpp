// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueSystem/DialogueComponent.h"
#include "GameFramework/Character.h"

// Sets default values for this component's properties
UDialogueComponent::UDialogueComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UDataTable> FindedPhrasesTable(TEXT("/Game/NewGameplay/Systems/DialogueSystem/DT_Phrases.DT_Phrases"));
	if (FindedPhrasesTable.Succeeded())
	{
		PhrasesTable = FindedPhrasesTable.Object;
	}
}


// Called when the game starts
void UDialogueComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());

	OnUpdateCurrentPhrase.AddDynamic(this, &UDialogueComponent::OnUpdateCurrentPhrase_Event);
}


// Called every frame
void UDialogueComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UDialogueComponent::FindVariantData(FPhraseTableData Phrase, FName VariantName, bool& bValid, FPhraseVariant& Result)
{
	bValid = false;
	Result = FPhraseVariant();

	for (FPhraseVariant Var : Phrase.Variants)
	{
		if (Var.Name == VariantName)
		{
			bValid = true;
			Result = Var;
			return;
		}
	}
}

void UDialogueComponent::SetCurrentPhrase(FName Phrase)
{
	if (Phrase != CurrentPhrase)
	{
		FName PrevPhrase = CurrentPhrase;
		CurrentPhrase = Phrase;
		OnUpdateCurrentPhrase.Broadcast(CurrentPhrase, PrevPhrase);
	}
}

void UDialogueComponent::NextPhrase()
{
	if (FPhraseTableData* CurrentPhraseTableRow = PhrasesTable->FindRow<FPhraseTableData>(CurrentPhrase, "Missing Phrase table row"))
	{
		if (CurrentPhraseTableRow->Variants.IsEmpty())
		{
			SetCurrentPhrase(CurrentPhraseTableRow->NextPhrase);
		}
		else
		{
			SelectVariant(SelectedVariantID);
		}
	}
}

bool UDialogueComponent::IsActivePhrase()
{
	return CurrentPhrase != "";
}

void UDialogueComponent::SelectVariant(int32 ID)
{
	if (FPhraseTableData* CurrentPhraseTableRow = PhrasesTable->FindRow<FPhraseTableData>(CurrentPhrase, "Missing Phrase table row"))
	{
		if (CurrentPhraseTableRow->Variants.IsValidIndex(ID))
		{
			FPhraseVariant VariantData = CurrentPhraseTableRow->Variants[ID];
			if (DialogueTarget)
			{
				DialogueTarget->OnSelectVariant.Broadcast(OwnerCharacter, VariantData.Name);
			}
			SetCurrentPhrase(VariantData.NextPhrase);
		}
	}
}

void UDialogueComponent::SetSelectedVariant(int32 VariantID)
{
	SelectedVariantID = -1;
	if (FPhraseTableData* CurrentPhraseTableRow = PhrasesTable->FindRow<FPhraseTableData>(CurrentPhrase, "Missing Phrase table row"))
	{
		int32 LastID = CurrentPhraseTableRow->Variants.Num() - 1;
		SelectedVariantID = FMath::Clamp(VariantID, 0, LastID);
	}
}

void UDialogueComponent::OffsetVariant(int32 Value)
{
	SetSelectedVariant(SelectedVariantID + Value);
}

void UDialogueComponent::OnUpdateCurrentPhrase_Event(FName NewPhrase, FName PrevPhrase)
{
	SetSelectedVariant(0);
	if (NewPhrase == "")
	{
		DialogueTarget = nullptr;
	}
}

void UDialogueComponent::BeginDialogue(FName StartPhrase, UDialogueComponent* TargetDialogueComponent)
{
	DialogueTarget = TargetDialogueComponent;
	SetCurrentPhrase(StartPhrase);
}

