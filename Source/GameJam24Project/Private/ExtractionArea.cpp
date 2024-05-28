// Fill out your copyright notice in the Description page of Project Settings.


#include "ExtractionArea.h"
#include "Components/BoxComponent.h"
#include "KoalaBabyCharacter.h"
#include "KoalaPlayerCharacter.h"


// Sets default values
AExtractionArea::AExtractionArea()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootBoxComp = CreateDefaultSubobject<UBoxComponent>(FName("Box Collision Comp"));
	RootBoxComp->SetGenerateOverlapEvents(true);
	RootBoxComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	RootBoxComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	SetRootComponent(RootBoxComp);
}

// Called when the game starts or when spawned
void AExtractionArea::BeginPlay()
{
	Super::BeginPlay();
	RootBoxComp->OnComponentBeginOverlap.AddDynamic(this, &AExtractionArea::OverlapBegin);
	RootBoxComp->OnComponentEndOverlap.AddDynamic(this, &AExtractionArea::OverlapEnd);
}

// Called every frame
void AExtractionArea::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AExtractionArea::OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(AKoalaPlayerCharacter::StaticClass())) {
		bIsPlayerInArea = true;
	}
	else if (OtherActor->IsA(AKoalaBabyCharacter::StaticClass())) {
		BabyKoalasNum++;
	}
}

void AExtractionArea::OverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->IsA(AKoalaPlayerCharacter::StaticClass())) {
		bIsPlayerInArea = false;
	}
	else if (OtherActor->IsA(AKoalaBabyCharacter::StaticClass())) {
		BabyKoalasNum--;
		if (BabyKoalasNum < 0) BabyKoalasNum = 0;
	}
}