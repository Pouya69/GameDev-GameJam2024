// Fill out your copyright notice in the Description page of Project Settings.


#include "MissionObjectivesWidget.h"
#include "MissionObjective.h"
#include "KoalaGameModeBase.h"
#include "Kismet/GameplayStatics.h"

void UMissionObjectivesWidget::NativeConstruct()
{
	Super::NativeConstruct();
	// MakeObjectivesWidget_Implementation();
	// MakeObjectivesWidget();

}

void UMissionObjectivesWidget::MakeObjectivesWidget_Implementation()
{
	AKoalaGameModeBase* GameMode = Cast<AKoalaGameModeBase>(UGameplayStatics::GetGameMode(GetOwningPlayer()->GetWorld()));
	CurrentObjectives.Empty();
	for (AMissionObjective* Obj : GameMode->MissionObjectives) {
		if (Obj->bIsObjectiveActive && !Obj->bIsObjectiveDone) {
			CurrentObjectives.Add(Obj);
			UE_LOG(LogTemp, Warning, TEXT("Objective Widget: %s"), *Obj->ObjectiveName);
		}
	}
	// MakeObjectivesWidget();
}
