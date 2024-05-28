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
	PrimaryActorTick.bCanEverTick = false;
	
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	SetRootComponent(BoxComponent);
	BoxComponent->InitBoxExtent(FVector(CollisionBoxExtent));

	BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BoxComponent->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	BoxComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Block);
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AFire::OnOverlapBegin);

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
	FHitResult OutHitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	TArray<FVector> HitVector;

	for(auto End : SpreadDirections )
	{
		const bool bLineTraceHasHit = GetWorld()->LineTraceSingleByChannel(OutHitResult, Start,  End , ECollisionChannel::ECC_GameTraceChannel1, Params);
		// DrawDebugLine(GetWorld(), Start, End, FColor::Red, true, 2.f);
		if(bLineTraceHasHit)
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
			SpawnedFire->Niagara = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NiagaraTemplate, Location + FVector(0, 0, NiagaraParticleZOffset), FRotator::ZeroRotator, FVector(NiagaraParticleScale));
			SpawnedFire->NiagaraTemplate = NiagaraTemplate;
		}
	}
}


void AFire::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& HitResult)
{
	const bool bIsAKoala = OtherActor->IsA(AKoalaBaseCharacter::StaticClass());
	if(bIsAKoala)
	{
		UGameplayStatics::ApplyDamage(OtherActor, 10.f, nullptr, this, UDamageType::StaticClass());
	}
}
