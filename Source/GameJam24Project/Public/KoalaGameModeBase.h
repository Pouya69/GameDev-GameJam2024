// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "KoalaGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class GAMEJAM24PROJECT_API AKoalaGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<class AMissionObjective*> MissionObjectives;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class AMissionObjective> ObjectiveClass;
	UFUNCTION(BlueprintCallable)
		bool CheckPlayerAndCompleteObjective(AActor* OtherActor, class AMissionObjective* Objective);

	class AKoalaPlayerCharacter* PlayerCharacter;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
