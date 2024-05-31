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
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
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

	UFUNCTION()
		void OnPlayerCharacterDeath();
	UFUNCTION()
		void OnBabyKoalaDeath();
	void DisablePlayerInput();

	UFUNCTION(BlueprintCallable)
		void GameOver(bool bWon, const FString& Message);
	UFUNCTION(BlueprintCallable)
		void EndGame();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Extraction")
		int ExtractionTimeSinceStart;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Extraction")
		int TimeLeftExtraction;
	void SetupTimerForEndGame();
	FTimerHandle TimerHandleExtraction;

public:

	// Fire creation
	FTimerHandle TimerHandleFireRandom;
	int FireActorsInLevel;
	TArray<class AKoalaBabyCharacter*> BabyCharacters;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Fire Random Creation")
		int CreateFireEverySeconds = 3;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Fire Random Creation")
		int MaxFiresAllowed = 30;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Fire Random Creation")
		float MinBabyRadiusSpawnFire = 1000;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Fire Random Creation")
		float MaxBabyRadiusSpawnFire = 5000;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Fire Random Creation")
		TSubclassOf<class AFire> FireClass;
	void CreateFireRandom();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
