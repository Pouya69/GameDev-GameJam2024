// Fill out your copyright notice in the Description page of Project Settings.


#include "KoalaGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "MissionObjective.h"
#include "KoalaPlayerCharacter.h"
#include "KoalaBabyCharacter.h"
#include "ExtractionArea.h"
#include "Fire.h"
#include "NavigationSystem.h"

void AKoalaGameModeBase::BeginPlay()
{
	TArray<AActor*> FindActors;
	PlayerCharacter = Cast<AKoalaPlayerCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	PlayerCharacter->DeathEvent.AddDynamic(this, &AKoalaGameModeBase::OnPlayerCharacterDeath);
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ObjectiveClass, FindActors);
	for (AActor* Actor : FindActors) {
		MissionObjectives.Add(Cast<AMissionObjective>(Actor));
	}
	FindActors.Empty();
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), BabyKoalaCharacterClass, FindActors);
	for (AActor* Actor : FindActors) {
		AKoalaBabyCharacter* BabyKoala = Cast<AKoalaBabyCharacter>(Actor);
		BabyCharacters.Add(BabyKoala);
		BabyKoala->DeathEvent.AddDynamic(this, &AKoalaGameModeBase::OnBabyKoalaDeath);
		BabyKoalasAlive++;
	}
	FindActors.Empty();
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), FireClass, FindActors);
	if (!FindActors.IsEmpty()) {
		FireActorsInLevel = FindActors.Num();
	}
	FindActors.Empty();
	ExtractionArea = Cast<AExtractionArea>(UGameplayStatics::GetActorOfClass(GetWorld(), ExtractionAreaClass));
	SetupTimerForEndGame();
	// Creating random fire actors
	FTimerDelegate FireDelegate;
	FireDelegate.BindLambda([&]() {
		if (FireActorsInLevel <= MaxFiresAllowed) CreateFireRandom();
	});
	GetWorldTimerManager().SetTimer(TimerHandleFireRandom, FireDelegate, CreateFireEverySeconds, true);

}


void AKoalaGameModeBase::CreateFireRandom()
{
	UNavigationSystemV1* NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	FNavLocation MoveLocationNav;
	const float RadiusRandom = FMath::RandRange(MinBabyRadiusSpawnFire, MaxBabyRadiusSpawnFire);
	AKoalaBabyCharacter* BabyCharacter = BabyCharacters[FMath::RandRange(0, BabyCharacters.Num() - 1)];
	// UE_LOG(LogTemp, Warning, TEXT("Random Baby: %s"), *BabyCharacter->GetName());
	const bool bFoundLocation = NavSystem->GetRandomReachablePointInRadius(BabyCharacter->GetActorLocation(), RadiusRandom, MoveLocationNav);
	if (!bFoundLocation) return;
	AFire* FireObjectSpawned = GetWorld()->SpawnActor<AFire>(FireClass, MoveLocationNav.Location, FRotator::ZeroRotator);
	TArray<AActor*> OutActors;
	FireObjectSpawned->GetOverlappingActors(OutActors, FireClass);
	if (!OutActors.IsEmpty()) {
		FireObjectSpawned->Destroy();
		return;
	}



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
	FString Message = FString("EVENT: You died...");
	GameOver(false, Message);
}

void AKoalaGameModeBase::OnBabyKoalaDeath()
{
	BabyKoalasAlive--;
	UE_LOG(LogTemp, Warning, TEXT("A baby koala has died. Left: %d"), BabyKoalasAlive);
	if (BabyKoalasAlive < MinBabyKoalasAliveNeeded) {
		FString Message = FString("You have failed to rescue the koalas...");
		GameOver(false, Message);
	}

	PlayerCharacter->UpdateKoalasAliveWidget();
}

void AKoalaGameModeBase::DisablePlayerInput()
{
	if (PlayerCharacter == nullptr) return;
	
	PlayerCharacter->DisableInput(PlayerCharacter->PlayerController);
	PlayerCharacter->SetActorTickEnabled(false);
	PlayerCharacter->PlayerController->UnPossess();
	
}

void AKoalaGameModeBase::GameOver(bool bWon, const FString& Message)
{
	DisablePlayerInput();
	GetWorldTimerManager().ClearAllTimersForObject(this);
	// TODO: Implement GameOver function
	if (bWon) {
		// Won the level
	}
	else {
		// Lost the level
	}

	UE_LOG(LogTemp, Warning, TEXT("GAME OVER: %s"), *Message);
}

void AKoalaGameModeBase::EndGame()
{
	UE_LOG(LogTemp, Warning, TEXT("Ending game..."), TimeLeftExtraction);

	bool bWon;
	FString Message = FString("You failed to save the koalas..."); // By default
	int KoalasSaved = ExtractionArea->GetBabyKoalasNumInArea();
	if (!PlayerCharacter->IsDead() && KoalasSaved >= MinBabyKoalasAliveNeeded) {
		bWon = true;
		if (ExtractionArea->IsPlayerInArea()) {
			// If player is also in area
			Message = FString("You saved the babies!");
		}
		else {
			// If everything is good but player is not in area themselves
			Message = FString("You saved the babies, but you sacrificed yourself in the way...");
		}
		Message.Append(FString("Koalas saved: ") + FString::FromInt(KoalasSaved));
	}
	PlayerCharacter->SetActorTickEnabled(false);
	if (APlayerController* PlayerController = Cast<APlayerController>(PlayerCharacter->GetController())) {
		PlayerController->DisableInput(PlayerController);
	}
	
	GameOver(false, Message);
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