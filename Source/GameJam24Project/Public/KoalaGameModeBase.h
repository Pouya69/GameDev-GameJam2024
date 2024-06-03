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
	UPROPERTY(VisibleAnywhere)
		TArray<class ABaseTree*> TreesInLevel;
	UPROPERTY(EditAnywhere)
		TArray<TSubclassOf<class ABaseTree>> TreeClasses;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int BabyKoalasAlive;
	int TotalBabyKoalasSinceStart;
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

	// End Game
	UPROPERTY(EditAnywhere, BlueprintReadWRite, Category = "End Game")
		bool bGameIsOver = true;
	void DisablePlayerInputAndRemoveWidgets();
	UFUNCTION(BlueprintCallable)
		void GameOver(bool bWon, const FString& Message, int KoalasSaved = 0);
	UFUNCTION(BlueprintCallable)
		void EndGame();
	UPROPERTY(EditAnywhere, Category = "End Game")
		FString NextLevelName;
	UPROPERTY(EditAnywhere, Category = "End Game")
		TSubclassOf<class UEndGameOverWidget> GameOverWidgetClass;
	UPROPERTY(VisibleAnywhere, Category = "End Game")
		class UEndGameOverWidget* GameOverWidget;
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
		float CreateFireEverySeconds = 3;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Fire Random Creation")
		int MaxFiresAllowed = 15;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Fire Random Creation")
		float MinBabyRadiusSpawnFire = 1000;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Fire Random Creation")
		float MaxBabyRadiusSpawnFire = 5000;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Fire Random Creation")
		TSubclassOf<class AFire> FireClass;
	void CreateFireRandom();

public:
	// Items placement
	FTimerHandle TimerHandleConsumablesRandom;
	int ConsumablesInLevel;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Consumable Random Creation")
		int MaxConsumablesAllowed = 10;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Consumable Random Creation")
		float CreateConsumableEverySeconds = 3;
	void CreateConsumableRandom();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Sound
	UPROPERTY(EditAnywhere, Category = "Sound")
		USoundBase* AtmosphereSound;
	UAudioComponent* AtmosphereSoundComp;
};
