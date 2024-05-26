// Fill out your copyright notice in the Description page of Project Settings.


#include "KoalaBaseCharacter.h"
#include "Components/CapsuleComponent.h"
#include "BaseTree.h"

// Sets default values
AKoalaBaseCharacter::AKoalaBaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	// PrimaryActorTick.bCanEverTick = true;
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

bool AKoalaBaseCharacter::AreThereAnyTreesAround(FHitResult& OutHitResult) const
{
	bool bResult = GetObjectsAround(OutHitResult, TreeDistanceCheck);
	if (bResult) {
		// To check if the hit actor was a tree specifically
		bResult = OutHitResult.GetActor()->IsA(ABaseTree::StaticClass());
	}
	return bResult;
}

bool AKoalaBaseCharacter::GetObjectsAround(FHitResult& OutHitResult, float RangeCheck) const
{
	FVector Start = GetActorLocation();
	FVector End = Start + (RangeCheck * GetActorForwardVector());
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	bool bResult = GetWorld()->SweepSingleByChannel(OutHitResult, Start, End, FQuat::Identity, ECollisionChannel::ECC_GameTraceChannel1, FCollisionShape::MakeSphere(RangeCheck), Params);
	if (bResult) {
		// To check whether the interacted object is the ABaseInteractableObject OR a character (baby koala for example)
		bResult = OutHitResult.GetActor()->IsA(ABaseInteractableObject::StaticClass()) || OutHitResult.GetActor()->IsA(ACharacter::StaticClass());
	}

	return bResult;
}
