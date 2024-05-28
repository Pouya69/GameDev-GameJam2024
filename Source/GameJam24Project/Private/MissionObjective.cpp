// Fill out your copyright notice in the Description page of Project Settings.


#include "MissionObjective.h"
#include "Kismet/GameplayStatics.h"
#include "MissionObjectivesWidget.h"
#include "KoalaPlayerCharacter.h"

AMissionObjective::AMissionObjective()
{
}


bool AMissionObjective::ObjectiveConditionsMet() const
{
	for (AMissionObjective* Mission : RelyingObjectives) {
		if (!Mission->bIsObjectiveDone) return false;
	}
	return true;
}

void AMissionObjective::ChangeObjectiveState()
{
	if (ObjectiveConditionsMet()) {
		bIsObjectiveDone = true;
		bIsObjectiveActive = false;
		for (AMissionObjective* Mission : ActivatingObjectivesWhenThisFinished) {
			// Do Below later
			/*if (Mission->ObjectiveConditionsMet()) {
				Mission->bIsObjectiveActive = false;
				Mission->bIsObjectiveDone = true;
				continue;
			}*/
			Mission->bIsObjectiveActive = true;
			Mission->bIsObjectiveDone = false;
			
		}
	}
	AKoalaPlayerCharacter* PlayerCharacter = Cast<AKoalaPlayerCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	PlayerCharacter->ObjectivesWidget->MakeObjectivesWidget_Implementation();
	PlayerCharacter->ObjectivesWidget->MakeObjectivesWidget();
	ChangedObjectiveState.Broadcast();
}

/*void AMissionObjective::ChangeObjectiveState_Implementation() {
	if (ObjectiveConditionsMet()) {
		bIsObjectiveDone = true;
	}
}*/