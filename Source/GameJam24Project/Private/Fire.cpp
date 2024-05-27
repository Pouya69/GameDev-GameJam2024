// Fill out your copyright notice in the Description page of Project Settings.


#include "Fire.h"
#include "Math.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UObject/ConstructorHelpers.h"


// Sets default values
AFire::AFire()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	SetRootComponent(BoxComponent);
	BoxComponent->InitBoxExtent(FVector(CollisionBoxExtent));

	BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BoxComponent->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	BoxComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Block);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Mesh-> SetupAttachment(BoxComponent);


}

// Called when the game starts or when spawned
void AFire::BeginPlay()
{
	Super::BeginPlay();

	FVector Start = GetActorLocation();

	// TODO Check where the safezone is so fire spread in that direction
	SpreadDirections = TArray<FVector>{
		Start +  FVector(0,  2 * CollisionBoxExtent, 0),
		Start +  FVector(2 * CollisionBoxExtent,  2 * CollisionBoxExtent, 0),
		Start +  FVector(2 * CollisionBoxExtent,  -2 * CollisionBoxExtent, 0),
		Start +  FVector(0,  -2 * CollisionBoxExtent, 0),
		Start +  FVector(2 * CollisionBoxExtent,  0, 0)
	};

	GetWorldTimerManager().SetTimer(SpreadTimer, this, &AFire::SpreadFire, SpreadTime, true);
	
}

// Called every frame
void AFire::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void AFire::SpreadFire()
{
	if(!Mesh->GetStaticMesh() || SpreadDirections.IsEmpty())
	{
		GetWorldTimerManager().ClearAllTimersForObject(this);
		return;	
	} 

	FVector Start = GetActorLocation();
	FHitResult OutHitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	TArray<FVector> HitVector;

	for(auto End : SpreadDirections )
	{
		bool bResult = GetWorld()->LineTraceSingleByChannel(OutHitResult, Start,  End , ECollisionChannel::ECC_GameTraceChannel1, Params);
		// DrawDebugLine(GetWorld(), Start, End, FColor::Red, true, 2.f);
		if(bResult)
		{
			HitVector.Add(End);
		}
		else
		{
			SpawnFire(End);
		}
	}

	for( auto Vector : HitVector)
	{
		SpreadDirections.Remove(Vector);
	}

	SpawnProbability += IncrementProbabilityRate;
	
}

void AFire::SpawnFire(FVector Location)
{
	if(FMath::RandRange(0,100) <= SpawnProbability)
	{
		AFire* SpawnedFire = GetWorld()->SpawnActor<AFire>(AFire::StaticClass(), Location, GetActorRotation());

		if(SpawnedFire)
		{
			SpawnedFire->Mesh->SetStaticMesh(Mesh->GetStaticMesh());
		}
	}
}
