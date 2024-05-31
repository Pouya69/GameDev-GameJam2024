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
#include "BaseTree.h"
#include "NavigationSystem.h"
#include "KoalaGameModeBase.h"
#include "KoalaBabyCharacter.h"


// Sets default values
AFire::AFire()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	SceneRootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Root Comp"));
	SetRootComponent(SceneRootComp);

}

// Called when the game starts or when spawned
void AFire::BeginPlay()
{
	Super::BeginPlay();

	LocationToSpawnFrom = GetActorLocation();

	// TODO Check where the safezone is so fire spread in that direction

	MakeFire(GetActorLocation());

	GetWorldTimerManager().SetTimer(SpreadTimer, this, &AFire::SpreadFire, SpreadTime, true);

	OnActorBeginOverlap.AddDynamic(this, &AFire::OnOverlapBegin);
	OnActorEndOverlap.AddDynamic(this, &AFire::OnOverlapEnd);
}

// Called every frame
void AFire::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void AFire::SpreadFire()
{
	FVector SpawnLocation = LocationToSpawnFrom;
	if (GetRandomLocation(SpawnLocation)) {
		SpawnFire(SpawnLocation);
	}
	SpawnProbability += IncrementProbabilityRate;
	
}

bool AFire::GetRandomLocation(FVector& OutLocation) const
{
	UNavigationSystemV1* NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	FNavLocation MoveLocationNav;
	bool bFoundLocation = NavSystem->GetRandomReachablePointInRadius(OutLocation, FireCreationRadius, MoveLocationNav);
	if (!bFoundLocation) {
		bFoundLocation = NavSystem->GetRandomReachablePointInRadius(GetActorLocation(), FireCreationRadius, MoveLocationNav);
		if (!bFoundLocation) return false;
	}
	OutLocation = MoveLocationNav.Location;
	return true;
}

void AFire::SpawnFire(FVector Location)
{

	if(FMath::RandRange(0,100) <= SpawnProbability)
	{
		MakeFire(Location);
		LocationToSpawnFrom = Location;
	}
}

void AFire::DestroyFire(UPrimitiveComponent* ComponentHit)
{
	TArray<USceneComponent*> ChildrenComps;
	ComponentHit->GetChildrenComponents(true, ChildrenComps);
	if (!ChildrenComps.IsEmpty()) {
		for (USceneComponent* Comp : ChildrenComps) Comp->DestroyComponent();
	}
	ComponentHit->DestroyComponent(false);
	TArray<UBoxComponent*> Comps;
	GetComponents(UBoxComponent::StaticClass(), Comps, true);
	if (Comps.IsEmpty()) {
		AKoalaGameModeBase* GameMode = Cast<AKoalaGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
		GameMode->FireActorsInLevel--;
		UE_LOG(LogTemp, Warning, TEXT("Destroying FIRE"));
		this->Destroy();
	}
}


void AFire::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
	// TODO Fix applydamage when moving into fire. Should stop applying directly but only trough the timer handler
	// const bool bIsAKoala = OtherActor->IsA(::StaticClass());
	const bool bTimerExists = GetWorldTimerManager(). TimerExists(DamageTimer);
	if(AKoalaBaseCharacter* KoalaBaseCharacter = Cast<AKoalaBaseCharacter>(OtherActor))
	{
		OverlapActors.Add(KoalaBaseCharacter);
		if(!bTimerExists)
		{
			GetWorldTimerManager().SetTimer(DamageTimer, this, &AFire::ApplyDamageTimer, 1.f, true, 0.f); 
		}
	}
	else if (ABaseTree* TreeObject = Cast<ABaseTree>(OtherActor)) {
		TreeObject->StartFire();
	}
}

void AFire::OnOverlapEnd(AActor* OverlappedActor, AActor* OtherActor)
{

	const bool bIsAKoala = OtherActor->IsA(AKoalaBaseCharacter::StaticClass());
	if(bIsAKoala)
	{
		OverlapActors.Remove(OtherActor);
		GetWorldTimerManager().ClearTimer(DamageTimer);
	}
}

float AFire::TakeDamage(float Damage, FDamageEvent const &DamageEvent, AController *InstigatedBy, AActor *DamageCauser)
{
	Destroy();
    return 0.0f;
}

void AFire::UpdateBoxCollisions()
{
	UpdateOverlaps(true);
}

void AFire::MakeFire(FVector Location)
{
	UBoxComponent* NewBoxComp = Cast<UBoxComponent>(AddComponentByClass(UBoxComponent::StaticClass(), true, GetTransform(), false));
	NewBoxComp->AttachToComponent(SceneRootComp, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	NewBoxComp->InitBoxExtent(FVector(CollisionBoxExtent));
	FVector BoxLoc = Location;
	BoxLoc.Z += NewBoxComp->GetScaledBoxExtent().Z / 2 + 5;
	NewBoxComp->SetWorldLocation(BoxLoc);
	NewBoxComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	NewBoxComp->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	NewBoxComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	NewBoxComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
	NewBoxComp->SetHiddenInGame(false, true);
	NewBoxComp->bMultiBodyOverlap = true;

	UNiagaraComponent* NewNiagara = UNiagaraFunctionLibrary::SpawnSystemAttached(NiagaraSystemManual, NewBoxComp, NAME_None, FVector::ZeroVector, NiagaraParticleRotation, EAttachLocation::SnapToTargetIncludingScale, false);
	NewNiagara->SetRelativeScale3D(NiagaraParticleScale);
	NewNiagara->SetHiddenInGame(false, true);
}

void AFire::ApplyDamageTimer()
{
	UpdateBoxCollisions();
	if (OverlapActors.IsEmpty())
	{
		GetWorldTimerManager().ClearAllTimersForObject(this);
		return;
	}
	for (AActor* Actor : OverlapActors) {
		UGameplayStatics::ApplyDamage(Actor, TickDamage, UGameplayStatics::GetPlayerController(GetWorld(), 0), this, UDamageType::StaticClass());
	}
	
}