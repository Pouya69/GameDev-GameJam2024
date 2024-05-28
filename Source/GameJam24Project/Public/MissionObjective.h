// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MissionObjective.generated.h"
/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FChangedObjectiveState);
UCLASS()
class GAMEJAM24PROJECT_API AMissionObjective : public AActor
{
	GENERATED_BODY()
public:
	AMissionObjective();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString ObjectiveDescription;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString ObjectiveName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Relying")
		TArray<AMissionObjective*> RelyingObjectives;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Relying")
		TArray<AMissionObjective*> ActivatingObjectivesWhenThisFinished;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bIsObjectiveDone = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bIsObjectiveActive = false;
	UFUNCTION(BlueprintCallable)
		bool ObjectiveConditionsMet() const;

	// Delegate signature
	UFUNCTION(BlueprintCallable)
		void ChangeObjectiveState();

	FChangedObjectiveState& OnChangedObjectiveState() { return ChangedObjectiveState; }
private:
	/** Broadcasts whenever the layer changes */
	FChangedObjectiveState ChangedObjectiveState;

};
