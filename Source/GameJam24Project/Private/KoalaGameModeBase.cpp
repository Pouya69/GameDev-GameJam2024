// Fill out your copyright notice in the Description page of Project Settings.


#include "KoalaGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "MissionObjective.h"
#include "KoalaPlayerCharacter.h"

void AKoalaGameModeBase::BeginPlay()
{
	TArray<AActor*> FindActors;
	PlayerCharacter = Cast<AKoalaPlayerCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ObjectiveClass, FindActors);
	for (AActor* Actor : FindActors) {
		MissionObjectives.Add(Cast<AMissionObjective>(Actor));
	}
	

	FindActors.Empty();
}


bool AKoalaGameModeBase::CheckPlayerAndCompleteObjective(AActor* OtherActor, AMissionObjective* Objective)
{
	if (Objective->bIsObjectiveDone) return true;
	if (OtherActor == nullptr) {
		// No need for Player Character Reference. We can check without it
		Objective->ChangeObjectiveState();
		return true;
	}
	if (!OtherActor->IsA(AKoalaPlayerCharacter::StaticClass())) return false;
	Objective->ChangeObjectiveState();
	return true;
}