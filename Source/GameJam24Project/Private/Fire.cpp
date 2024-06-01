// Fill out your copyright notice in the Description page of Project Settings.


#include "Fire.h"
#include "Math.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Components/BoxComponent.h"
#include "Components/SplineComponent.h"
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
#include "Consumable.h"


// Sets default values
AFire::AFire()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
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
	GetWorldTimerManager().SetTimer(DamageTimer, this, &AFire::ApplyDamageTimer, 1.f, true, 0.f);
}

// Called every frame
void AFire::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void AFire::SpreadFire()
{
	/* UpdateBoxCollisions();
	if (GetRandomLocation(LocationToSpawnFrom)) {
		SpawnFire(LocationToSpawnFrom);
		
	}
	SpawnProbability += IncrementProbabilityRate; */
	
}

bool AFire::GetRandomLocation(FVector& OutLocation)
{
	/* FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	FHitResult HitResult;
	const FVector SpawnLocFireExists = OutLocation + (FVector::UpVector * FireCreationUpwardsCheck);
	const bool bFoundLocationUpFireExists = GetWorld()->SweepSingleByChannel(HitResult, SpawnLocFireExists, SpawnLocFireExists + (FVector::UpVector * FireCreationUpwardsCheck / 3), FQuat::Identity, ECC_WorldStatic, FCollisionShape::MakeSphere(20.f), Params);
	if (bFoundLocationUpFireExists) {
		if (HitResult.GetActor()->IsA(AFire::StaticClass())) {
			bIsCheckingOnTree = false;
			OutLocation = GetActorLocation();
			return false;
		}
	}
	const float RandomUpwards = FMath::RandRange(FireCreationUpwardsCheck / 3, FireCreationUpwardsCheck);
	const FVector SpawnLoc = OutLocation + (FVector::UpVector * RandomUpwards);
	const bool bFoundLocationUp = GetWorld()->SweepSingleByChannel(HitResult, SpawnLoc, SpawnLoc + (FVector::UpVector * RandomUpwards/3), FQuat::Identity, ECC_WorldStatic, FCollisionShape::MakeSphere(20.f), Params);

	if (bFoundLocationUp) {
		if (HitResult.GetActor()->IsA(TreeClass)) {
			bIsCheckingOnTree = true;
			UE_LOG(LogTemp, Warning, TEXT("UP"));
			OutLocation = HitResult.ImpactPoint;
			return true;
		}
		if (bIsCheckingOnTree) {
			bIsCheckingOnTree = false;
			OutLocation = GetActorLocation();
			UE_LOG(LogTemp, Warning, TEXT("Reset"));
		}
	}
	else {
		if (bIsCheckingOnTree) {
			bIsCheckingOnTree = false;
			OutLocation = GetActorLocation();
			UE_LOG(LogTemp, Warning, TEXT("Reset"));
		}
	} */
	/*
	bool bIsOnTree = false;
	for (const AActor* Actor : OverlapActors) {
		if (Actor->IsA(TreeClass)) {
			bIsOnTree = true;
			break;
		}
	}
	if (bIsOnTree) {
		
		// OutLocation = GetActorLocation();
	}
	*/
	UNavigationSystemV1* NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	FNavLocation MoveLocationNav;
	bool bFoundLocation = NavSystem->GetRandomReachablePointInRadius(LocationToSpawnFrom, FireCreationRadius, MoveLocationNav);
	if (!bFoundLocation) {
		/*if (bIsOnTree) {
			const FVector SpawnLoc2 = LocationToSpawnFrom + (FVector::RightVector * FireCreationUpwardsCheck);
			const bool bFoundLocationRight = GetWorld()->SweepMultiByChannel(HitResults, SpawnLoc2, SpawnLoc2 + (FVector::RightVector * FireCreationUpwardsCheck / 2), FQuat::Identity, ECC_WorldStatic, FCollisionShape::MakeSphere(20.f), Params);
			if (bFoundLocationRight) {
				for (const FHitResult HitResult : HitResults) {
					OutLocations.Add(HitResult.ImpactPoint);
				}
				return true;
			}
			return false;
		}*/
		
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
	UE_LOG(LogTemp, Warning, TEXT("Overlapping %s"), *OtherActor->GetName());
	// TODO Fix applydamage when moving into fire. Should stop applying directly but only trough the timer handler
	// const bool bIsAKoala = OtherActor->IsA(::StaticClass());
	if (OtherActor->IsA(ACharacter::StaticClass()) || OtherActor->IsA(ABaseTree::StaticClass()) || OtherActor->IsA(AConsumable::StaticClass())) {
		OverlapActors.Add(OtherActor);
	}
	else {
		return;
	}
	const bool bTimerExists = GetWorldTimerManager().TimerExists(DamageTimer);
	if (bTimerExists) return;
	if(AKoalaBaseCharacter* KoalaBaseCharacter = Cast<AKoalaBaseCharacter>(OtherActor))
	{
		if(!bTimerExists)
		{
			GetWorldTimerManager().SetTimer(DamageTimer, this, &AFire::ApplyDamageTimer, 1.f, true, 0.f);
		}
	}
	else if (ABaseTree* TreeObject = Cast<ABaseTree>(OtherActor)) {
		InitializeSpline(TreeObject->SplineComponent);
		/* if(TargetSpline)
		{
			GetWorldTimerManager().SetTimer(SplineTimer, this, &AFire::FollowSpline, 1.f, true, 0.f);
		} */
		//TreeObject->StartFire();
	}
	else if (AConsumable* Consumable = Cast<AConsumable>(OtherActor)) {
		if(!bTimerExists)
		{
			GetWorldTimerManager().SetTimer(DamageTimer, this, &AFire::ApplyDamageTimer, 1.f, true, 0.f);
		}
	}
}

void AFire::OnOverlapEnd(AActor* OverlappedActor, AActor* OtherActor)
{

	const bool bIsAKoala = OtherActor->IsA(AKoalaBaseCharacter::StaticClass());
	if(bIsAKoala)
	{
		OverlapActors.Remove(OtherActor);
		// GetWorldTimerManager().ClearTimer(DamageTimer);
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

	UpdateBoxCollisions();
}

void AFire::ApplyDamageTimer()
{
	UpdateBoxCollisions();
	OverlapActors.Empty();
	GetOverlappingActors(OverlapActors);
	if (OverlapActors.IsEmpty())
	{
		GetWorldTimerManager().ClearTimer(DamageTimer);
		return;
	}
	const int Num = OverlapActors.Num();
	AKoalaBaseCharacter* KoalaBasePlayer = Cast<AKoalaBaseCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if (!KoalaBasePlayer) {
		GetWorldTimerManager().ClearAllTimersForObject(this);
		return;
	}
	if (KoalaBasePlayer->IsDead()) {
		GetWorldTimerManager().ClearAllTimersForObject(this);
		return;
	}
	for (int i = 0; i < Num; i++) {
		if (!OverlapActors.IsValidIndex(i) || OverlapActors[i] == nullptr) {
			OverlapActors.Empty();
			GetWorldTimerManager().ClearTimer(DamageTimer);
			return;
		}
		// UE_LOG(LogTemp, Warning, TEXT("%s"), *Actor->GetName());
		UGameplayStatics::ApplyDamage(OverlapActors[i], TickDamage, KoalaBasePlayer->GetController(), KoalaBasePlayer, UDamageType::StaticClass());
	}
	
}


void AFire::InitializeSpline(class USplineComponent* SplineComponent)
{
	TargetSpline = SplineComponent;
	if(SplineComponent)
	{
        FVector Start = TargetSpline->GetLocationAtTime(0, ESplineCoordinateSpace::World, true);
        FVector End = TargetSpline->GetLocationAtTime(SplineComponent->Duration, ESplineCoordinateSpace::World, true);
		float Length = End.Z - Start.Z;
		float ActorsNeeded = Length / CollisionBoxExtent;

		float Delay = SplineComponent->Duration / ActorsNeeded;
		UE_LOG(LogTemp, Warning, TEXT("Length %f ActorsNeeded %f Delay %f"), Length, ActorsNeeded, Delay);

		for(int i=1; i<=ActorsNeeded;i++)
		{
			CalculateSplineLocations(Delay * i);
		}

		
		GetWorldTimerManager().SetTimer(SplineTimer,this, &AFire::SpawnSplineFire, Delay, true);

	}
}

void AFire::CalculateSplineLocations(float Time)
{
	if (TargetSpline)
    {
        FVector SplineLocation = TargetSpline->GetLocationAtTime(Time, ESplineCoordinateSpace::World, true);

        FRotator SplineRotation = TargetSpline->GetRotationAtTime(Time, ESplineCoordinateSpace::World);

		FTimerDelegate FireDelegate;
		SplineLocations.Add(TTuple<FVector, FRotator>(SplineLocation, SplineRotation));
		
    }
}


void AFire::SpawnSplineFire()
{
	if(SplineLocations.IsEmpty())
	{
		GetWorldTimerManager().ClearTimer(SplineTimer);
		return;
	}

	TTuple<FVector, FRotator> Location = SplineLocations[0];
	
	UE_LOG(LogTemp, Warning, TEXT("%s %s"), *Location.Get<0>().ToString(), *Location.Get<1>().ToString());
	AFire* NewFire = GetWorld()->SpawnActor<AFire>(FireClass, Location.Get<0>(), Location.Get<1>());
	if(NewFire)
	{
		NewFire->SetActorEnableCollision(false);
	}

	SplineLocations.RemoveAt(0);

}