// Fill out your copyright notice in the Description page of Project Settings.


#include "BabyAIController.h"
#include "KoalaBabyCharacter.h"

void ABabyAIController::BeginPlay()
{
	Super::BeginPlay();
	BabyCharacterRef = Cast<AKoalaBabyCharacter>(GetPawn());
	if (CustomBT) {
		RunBehaviorTree(CustomBT);
	}
}
