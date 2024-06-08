// This is a free project


#include "ExtractionReadySetter.h"
#include "Kismet/GameplayStatics.h"
#include "KoalaGameModeBase.h"

// Sets default values
AExtractionReadySetter::AExtractionReadySetter()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AExtractionReadySetter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AExtractionReadySetter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AExtractionReadySetter::ExtractNow() {
	AKoalaGameModeBase* GameMode = Cast<AKoalaGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	GameMode->TimeLeftExtraction = ExtractionNewTime;
}