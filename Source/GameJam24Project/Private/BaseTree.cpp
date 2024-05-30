// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseTree.h"

ABaseTree::ABaseTree()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABaseTree::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseTree::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseTree::Interact() 
{
	UE_LOG(LogTemp, Warning, TEXT("Interacting tree..."));
}

void ABaseTree::StartFire()
{
	if (bIsOnFire) return;  // If already on fire ignore
	bIsOnFire = true;
	FTimerDelegate TreeFireDelegate;
	TreeFireDelegate.BindLambda([&]() {
		// TODO: Expand fire and create fire objects and etc.
		// TODO: Also have an end condition when the tree is fully on fire
	});

	GetWorldTimerManager().SetTimer(TreeFireTimer, TreeFireDelegate, ExpandFireEverySeconds, true);

}

