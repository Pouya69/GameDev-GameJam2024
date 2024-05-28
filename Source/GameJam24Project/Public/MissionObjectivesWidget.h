// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MissionObjectivesWidget.generated.h"

/**
 * 
 */
UCLASS()
class GAMEJAM24PROJECT_API UMissionObjectivesWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	// TODO: Make event to use in blueprint for creation
	DECLARE_EVENT(FLayerViewModel, FMakeObjectives)
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void MakeObjectivesWidget();

	FMakeObjectives& OnMakeObjectives() { return MakeObjectives; }

	virtual void MakeObjectivesWidget_Implementation();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<class AMissionObjective*> CurrentObjectives;

private:
	FMakeObjectives MakeObjectives;
	
};
