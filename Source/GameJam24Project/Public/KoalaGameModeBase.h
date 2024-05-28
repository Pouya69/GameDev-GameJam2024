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
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<class AMissionObjective*> MissionObjectives;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int BabyKoalasAlive;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int MinBabyKoalasAliveNeeded = 2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class AMissionObjective> ObjectiveClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class AExtractionArea> ExtractionAreaClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class AKoalaBabyCharacter> BabyKoalaCharacterClass;
	UFUNCTION(BlueprintCallable)
		bool CheckPlayerAndCompleteObjective(AActor* OtherActor, class AMissionObjective* Objective);

	class AKoalaPlayerCharacter* PlayerCharacter;
	class AExtractionArea* ExtractionArea;

	void OnPlayerCharacterDeath();
	void OnBabyKoalaDeath();

	UFUNCTION(BlueprintCallable)
		void GameOver(bool bWon, const FString& Message);
	UFUNCTION(BlueprintCallable)
		void EndGame();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
