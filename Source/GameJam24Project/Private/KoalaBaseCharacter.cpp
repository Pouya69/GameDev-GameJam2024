// Fill out your copyright notice in the Description page of Project Settings.


#include "KoalaBaseCharacter.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AKoalaBaseCharacter::AKoalaBaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CapsuleComp = Cast<UCapsuleComponent>(GetRootComponent());
}

// Called when the game starts or when spawned
void AKoalaBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	
}

// Called every frame
void AKoalaBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}