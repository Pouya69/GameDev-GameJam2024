// Fill out your copyright notice in the Description page of Project Settings.


#include "MissionObjectiveItemWidget.h"
#include "MissionObjective.h"

void UMissionObjectiveItemWidget::NativeConstruct()
{
	Super::NativeConstruct();
	// MissionObjectiveLinked->OnChangedObjectiveState().AddDynamic(this, &UMissionObjectiveItemWidget::ObjectiveDone);
}

FString& UMissionObjectiveItemWidget::GetTitle() const
{
	return MissionObjectiveLinked->ObjectiveName;
}

FString& UMissionObjectiveItemWidget::GetDescription() const
{
	return MissionObjectiveLinked->ObjectiveDescription;
}

void UMissionObjectiveItemWidget::MakeObjectiveItemWidget(AMissionObjective* MissionObjective)
{
	if (!MissionObjective) return;
	MissionObjectiveLinked = MissionObjective;
}
