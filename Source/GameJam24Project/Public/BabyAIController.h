// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BabyAIController.generated.h"

/**
 * 
 */
UCLASS()
class GAMEJAM24PROJECT_API ABabyAIController : public AAIController
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category="Custom AI")
		UBehaviorTree* CustomBT;
	
	class AKoalaBabyCharacter* BabyCharacterRef;
protected:
	virtual void BeginPlay() override;
};
