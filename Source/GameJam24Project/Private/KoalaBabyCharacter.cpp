// Fill out your copyright notice in the Description page of Project Settings.


#include "KoalaBabyCharacter.h"
#include "Components/CapsuleComponent.h"


AKoalaBabyCharacter::AKoalaBabyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	CapsuleComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Block);
}

void AKoalaBabyCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AKoalaBabyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// UE_LOG(LogTemp, Warning, TEXT("Baby fire: %d"), IsOnFire() ? 1 : 0);
}

void AKoalaBabyCharacter::Die()
{
	GetController()->Destroy();
	Super::Die();
}
