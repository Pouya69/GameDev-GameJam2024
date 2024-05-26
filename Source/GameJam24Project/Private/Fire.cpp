// Fill out your copyright notice in the Description page of Project Settings.


#include "Fire.h"
#include "Engine/World.h"

// Sets default values
AFire::AFire()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AFire::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFire::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);  

	/* TODO:
	1. Each tick check if tile next is free in 5/8 directions (method IsSpaceFree)
	2. If tile is free, spawn actor (method SpawnFire)
	3. If all are occupied, interrupt tick, for performances (PrimaryActorTick.bCanEverTick = false;)
	*/

}

bool AFire::IsSpaceFree(FVector Location)
{
	/* TODO:
	1. bool SweepSingleByChannel(
		FHitResult& OutHit, 
		const FVector& Start, 
		const FVector& End, 
		const FQuat& Rot, 
		ECollisionChannel TraceChannel, 
		const FCollisionShape& CollisionShape) const

	2. Basically just that, set up output params, if nullptr we can spawn. where to get instance of world for performances? UWorld* World{GetWorld()};
	3. If objects can be set on fire, this also returns like space is free. EXTRA
	*/

    return false;
}

void AFire::SpawnFire()
{
	/* TODO:
	1. Again get world reference and simply call SpawnActorAtLocation or similar?
	2. Probability to spawn
	3. If you want to set something on fire, should have actor to fire up and place tag? EXTRA
	*/

}
