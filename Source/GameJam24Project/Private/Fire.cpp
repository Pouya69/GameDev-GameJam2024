// Fill out your copyright notice in the Description page of Project Settings.


#include "Fire.h"
#include "Math.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Components/BoxComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "KoalaBaseCharacter.h"
#include "Engine/DamageEvents.h"


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
	BoxComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);

	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AFire::OnOverlapBegin);
	BoxComponent->OnComponentEndOverlap.AddDynamic(this, &AFire::OnOverlapEnd);

	Niagara = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
	Niagara->SetupAttachment(BoxComponent);

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
	if(!Niagara || SpreadDirections.IsEmpty())
	{
		GetWorldTimerManager().ClearAllTimersForObject(this);
		return;	
	} 

	FVector Start = GetActorLocation();
	TArray<FHitResult> OutHitResults;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	TArray<FVector> HitVector;

	for(auto End : SpreadDirections )
	{
		const bool bLineTraceHasHit = GetWorld()->LineTraceMultiByChannel(OutHitResults, Start,  End , ECollisionChannel::ECC_WorldStatic, Params);
		bool bShouldSpawn = true;
		if(bLineTraceHasHit)
		{
			for (FHitResult HitResult : OutHitResults) {
				if (HitResult.GetActor()->IsA(AFire::StaticClass())) {
					HitVector.Add(End);
					bShouldSpawn = false;
					break;
				}
			}
		}
		if (bShouldSpawn) {
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
		AFire* SpawnedFire = GetWorld()->SpawnActor<AFire>(FireClass, Location, GetActorRotation());
	}
}


void AFire::OnOverlapBegin( UPrimitiveComponent* OverlappedComp,  AActor* OtherActor,  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& HitResult)
{
	// TODO Fix applydamage when moving into fire. Should stop applying directly but only trough the timer handler
	const bool bIsAKoala = OtherActor->IsA(AKoalaBaseCharacter::StaticClass());
	
	const bool bTimerExists = GetWorldTimerManager(). TimerExists(DamageTimer);
	if(bIsAKoala)
	{
		bIsOverlapping = true;
		if(!bTimerExists)
		{
			ActorToDamage = OtherActor;
			GetWorldTimerManager().SetTimer(DamageTimer, this, &AFire::ApplyDamageTimer, 1.f, true, 0.f); 
		}
	}
}

void AFire::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	const bool bIsAKoala = OtherActor->IsA(AKoalaBaseCharacter::StaticClass());
	if(bIsAKoala)
	{
		bIsOverlapping = false;	
	}
}


void AFire::ApplyDamageTimer()
{
	if(!bIsOverlapping)
	{
		GetWorldTimerManager().ClearTimer(DamageTimer);
		return;
	}
	UGameplayStatics::ApplyDamage(ActorToDamage, Damage, nullptr, this, UDamageType::StaticClass());
}