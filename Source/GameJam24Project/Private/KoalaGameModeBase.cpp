// Fill out your copyright notice in the Description page of Project Settings.


#include "KoalaGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "MissionObjective.h"
#include "KoalaPlayerCharacter.h"
#include "KoalaBabyCharacter.h"
#include "ExtractionArea.h"
#include "Fire.h"
#include "NavigationSystem.h"
#include "BaseTree.h"
#include "EndGameOverWidget.h"
#include "Components/AudioComponent.h"


void AKoalaGameModeBase::BeginPlay()
{
	
	/*UGameplayStatics::GetAllActorsOfClass(GetWorld(), ObjectiveClass, FindActors);
	if (!FindActors.IsEmpty()) {
		for (AActor* Actor : FindActors) {
			MissionObjectives.Add(Cast<AMissionObjective>(Actor));
		}
		FindActors.Empty();
	}*/
	if (GetWorld() == nullptr) {
		return;
	}
	TArray<AActor*> FindActors;
	PlayerCharacter = Cast<AKoalaPlayerCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	PlayerCharacter->DeathEvent.AddDynamic(this, &AKoalaGameModeBase::OnPlayerCharacterDeath);
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), BabyKoalaCharacterClass, FindActors);
	if (!FindActors.IsEmpty()) {
		for (AActor* Actor : FindActors) {
			AKoalaBabyCharacter* BabyKoala = Cast<AKoalaBabyCharacter>(Actor);
			BabyCharacters.Add(BabyKoala);
			BabyKoala->DeathEvent.AddDynamic(this, &AKoalaGameModeBase::OnBabyKoalaDeath);
			BabyKoalasAlive++;
		}
		FindActors.Empty();
	}
	TotalBabyKoalasSinceStart = BabyKoalasAlive;
	UE_LOG(LogTemp, Warning, TEXT("Total: %d"), TotalBabyKoalasSinceStart);
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), FireClass, FindActors);
	if (!FindActors.IsEmpty()) {
		FireActorsInLevel = FindActors.Num();
	}
	FindActors.Empty();
	for (TSubclassOf<ABaseTree> TreeClass : TreeClasses) {
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), TreeClass, FindActors);
		for (AActor* Actor : FindActors) {
			TreesInLevel.Add(Cast<ABaseTree>(Actor));
		}
	}
	FindActors.Empty();
	ExtractionArea = Cast<AExtractionArea>(UGameplayStatics::GetActorOfClass(GetWorld(), ExtractionAreaClass));
	SetupTimerForEndGame();
	// Creating random fire actors
	FTimerDelegate FireDelegate;
	FireDelegate.BindLambda([&]() {
		// UE_LOG(LogTemp, Warning, TEXT("FireActorsInLevel: %d MaxFiresAllowed: %d"), FireActorsInLevel, MaxFiresAllowed);
		if (FireActorsInLevel < MaxFiresAllowed) CreateFireRandom();
	});

	GetWorldTimerManager().SetTimer(TimerHandleFireRandom, FireDelegate, CreateFireEverySeconds, true);
	// Creating random consumables
	if (!TreesInLevel.IsEmpty()) {
		if (MaxConsumablesAllowed > TreesInLevel.Num()) MaxConsumablesAllowed = TreesInLevel.Num();  // No more than the amount of trees in level

		FTimerDelegate ConsumableDelegate;
		ConsumableDelegate.BindLambda([&]() {
			if (ConsumablesInLevel < MaxConsumablesAllowed) CreateConsumableRandom();
			});
		
		GetWorldTimerManager().SetTimer(TimerHandleConsumablesRandom, ConsumableDelegate, CreateConsumableEverySeconds, true);
	}
	bGameIsOver = false;
	if (AtmosphereSound) {
		AtmosphereSoundComp = UGameplayStatics::SpawnSound2D(GetWorld(), AtmosphereSound);
	}
	
}


void AKoalaGameModeBase::CreateFireRandom()
{
	if (BabyCharacters.IsEmpty() || GetWorld() == nullptr) return;
	
	UNavigationSystemV1* NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	FNavLocation MoveLocationNav;
	const float RadiusRandom = FMath::RandRange(MinBabyRadiusSpawnFire, MaxBabyRadiusSpawnFire);
	AKoalaBabyCharacter* BabyCharacter = BabyCharacters[FMath::RandRange(0, BabyCharacters.Num() - 1)];
	// UE_LOG(LogTemp, Warning, TEXT("Random Baby: %s"), *BabyCharacter->GetName());
	const bool bFoundLocation = NavSystem->GetRandomPointInNavigableRadius(BabyCharacter->GetActorLocation(), RadiusRandom, MoveLocationNav);
	if (!bFoundLocation) return;
	AFire* FireObjectSpawned = GetWorld()->SpawnActor<AFire>(FireClass, MoveLocationNav.Location, FRotator::ZeroRotator);
	
	TArray<AActor*> OutActors;
	FireObjectSpawned->GetOverlappingActors(OutActors, FireClass);
	if (!OutActors.IsEmpty()) {
		FireObjectSpawned->Destroy();
		return;
	}
	if (FireObjectSpawned)
	{
		FireActorsInLevel++;
	}



}


void AKoalaGameModeBase::CreateConsumableRandom() {
	if (TreesInLevel.IsEmpty()) return;
	const int i = FMath::RandRange(0, TreesInLevel.Num()-1);
	ABaseTree* TreeObj = TreesInLevel[i];
	if (TreeObj->bCanSpawnFruit && !TreeObj->AlreadyHasConsumableOnTree()) {
		TreeObj->SpawnConsuamble();
		ConsumablesInLevel++;
	}
	// TreesInLevel[FMath::RandRange(0, TreesInLevel.Num()-1)]->SpawnConsuamble();
}


bool AKoalaGameModeBase::CheckPlayerAndCompleteObjective(AActor* OtherActor, AMissionObjective* Objective)
{
	if (!Objective) {
		UE_LOG(LogTemp, Error, TEXT("Objective Error in CheckPlayerAndCompleteObjective in GameMode"));
		return false;
	}
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

void AKoalaGameModeBase::OnPlayerCharacterDeath()
{
	FString Message = FString("You died...");
	GameOver(false, Message);
}

void AKoalaGameModeBase::OnBabyKoalaDeath()
{
	BabyKoalasAlive--;
	// UE_LOG(LogTemp, Warning, TEXT("A baby koala has died. Left: %d"), BabyKoalasAlive);
	if (BabyKoalasAlive < MinBabyKoalasAliveNeeded) {
		FString Message = FString("You have failed to rescue the koalas...");
		GameOver(false, Message);
	}

	PlayerCharacter->UpdateKoalasAliveWidget();
}

void AKoalaGameModeBase::DisablePlayerInputAndRemoveWidgets()
{
	if (PlayerCharacter == nullptr) return;
	PlayerCharacter->RemoveAllPlayerWidgets();
	PlayerCharacter->DisableInput(PlayerCharacter->PlayerController);
	PlayerCharacter->SetActorTickEnabled(false);
	PlayerCharacter->PlayerController->UnPossess();
	
}

void AKoalaGameModeBase::GameOver(bool bWon, const FString& Message, int KoalasSaved)
{
	if (!BabyCharacters.IsEmpty()) {
		for (AKoalaBabyCharacter* BabyChar : BabyCharacters) {
			if (BabyChar != nullptr) {
				BabyChar->Die();
			}
		}
	}
	
	bGameIsOver = true;
	PlayerCharacter->SetActorTickEnabled(false);
	PlayerCharacter->DisableInput(PlayerCharacter->PlayerController);
	DisablePlayerInputAndRemoveWidgets();
	GetWorldTimerManager().ClearTimer(TimerHandleConsumablesRandom);
	GetWorldTimerManager().ClearTimer(TimerHandleExtraction);
	GetWorldTimerManager().ClearTimer(TimerHandleFireRandom);
	if (AtmosphereSoundComp) {
		AtmosphereSoundComp->SetActive(false);
	}
	// GetWorldTimerManager().ClearAllTimersForObject(this);
	
	// if (PlayerCharacter->BasicPlayerWidget)	PlayerCharacter->BasicPlayerWidget->RemoveFromParent();
	// if (PlayerCharacter->ObjectivesWidget)	PlayerCharacter->ObjectivesWidget->RemoveFromParent();
	int Stars = 0;
	if (bWon) {
		Stars = KoalasSaved >= TotalBabyKoalasSinceStart ? 3 : (KoalasSaved == MinBabyKoalasAliveNeeded ? 1 : 2);
	}
	if (GameOverWidgetClass) {
		GameOverWidget = CreateWidget<UEndGameOverWidget>(PlayerCharacter->PlayerController, GameOverWidgetClass, FName("Game Over Widget"));
		if (GameOverWidget) {
			PlayerCharacter->PlayerController->bShowMouseCursor = true;
			GameOverWidget->BuildGameOverWidget(bWon, Message, NextLevelName, Stars);
			GameOverWidget->AddToViewport();
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("GAME OVER: %s"), *Message);
	
}

void AKoalaGameModeBase::EndGame()
{
	// UE_LOG(LogTemp, Warning, TEXT("Ending game..."), TimeLeftExtraction);

	bool bWon = false;
	FString Message = FString("You failed to save the koalas..."); // By default
	int KoalasSaved = ExtractionArea->GetBabyKoalasNumInArea();
	UE_LOG(LogTemp, Warning, TEXT("Ending game... Koalas Saved: %d"), KoalasSaved);
	if (!PlayerCharacter->IsDead() && KoalasSaved >= MinBabyKoalasAliveNeeded) {
		bWon = true;
		if (ExtractionArea->IsPlayerInArea()) {
			// If player is also in area
			Message = FString("You saved the babies!");
		}
		else {
			// If everything is good but player is not in area themselves
			Message = FString("You saved the babies but you got left out...");
		}
		// Message.Append(FString(" Koalas saved: ") + FString::FromInt(KoalasSaved));
	}
	
	GameOver(bWon, Message, KoalasSaved);
}

void AKoalaGameModeBase::SetupTimerForEndGame()
{
	TimeLeftExtraction = ExtractionTimeSinceStart;
	FTimerDelegate TimerDelegateExtraction;
	TimerDelegateExtraction.BindLambda([&]() {
		TimeLeftExtraction--;
		// UE_LOG(LogTemp, Warning, TEXT("Time Left: %d"), TimeLeftExtraction);
		if (TimeLeftExtraction < 0) {
			GetWorldTimerManager().ClearTimer(TimerHandleExtraction);
			EndGame();
		}
	});
	GetWorldTimerManager().SetTimer(TimerHandleExtraction, TimerDelegateExtraction, 1, true);
}
