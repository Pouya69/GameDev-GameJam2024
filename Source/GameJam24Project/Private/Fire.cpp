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
#include "Components/AudioComponent.h"


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

	// OnActorBeginOverlap.AddDynamic(this, &AFire::OnOverlapBegin);
	// OnActorEndOverlap.AddDynamic(this, &AFire::OnOverlapEnd);
	GetWorldTimerManager().SetTimer(DamageTimer, this, &AFire::ApplyDamageTimer, 1.f, true, 0.f);
	if (FireSound) {
		FireSoundComp = UGameplayStatics::SpawnSoundAtLocation(GetWorld(), FireSound, GetActorLocation());
	}
	FireDestroyAudioComp = UGameplayStatics::SpawnSoundAtLocation(GetWorld(), FirePutoutSound, GetActorLocation(), FRotator::ZeroRotator, 1, 1, 1, nullptr, nullptr, false);
	if(FireDestroyAudioComp) {
		FireDestroyAudioComp->SetActive(false);
	}
	
}

// Called every frame
void AFire::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void AFire::SpreadFire()
{
	UpdateBoxCollisions();
	if (GetRandomLocation(LocationToSpawnFrom)) {
		SpawnFire(LocationToSpawnFrom);
		
	}
	SpawnProbability += IncrementProbabilityRate;
	
}

bool AFire::GetRandomLocation(FVector& OutLocation)
{
	UNavigationSystemV1* NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	FNavLocation MoveLocationNav;
	bool bFoundLocation = NavSystem->GetRandomReachablePointInRadius(LocationToSpawnFrom, FireCreationRadius, MoveLocationNav);
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

void AFire::DestroyFire(UPrimitiveComponent* ComponentHit, bool bForceDestroy)
{
	if (bForceDestroy) {
		TArray<USceneComponent*> ChildrenComps;
		ComponentHit->GetChildrenComponents(true, ChildrenComps);
		if (!ChildrenComps.IsEmpty()) {
			for (USceneComponent* Comp : ChildrenComps) Comp->DestroyComponent();
		}
	}
	if (FireDestroyAudioComp) {
		FireDestroyAudioComp->SetWorldLocation(ComponentHit->GetComponentLocation());
		FireDestroyAudioComp->SetActive(true, true);
		FireDestroyAudioComp->Play();
	}
	TArray<USceneComponent*> ChildrenComps;
	ComponentHit->GetChildrenComponents(true, ChildrenComps);
	if (!ChildrenComps.IsEmpty()) {
		for (USceneComponent* Comp : ChildrenComps) Comp->DestroyComponent();
	}
	/*if (FirePutoutSound) {
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), FirePutoutSound, ComponentHit->GetComponentLocation());
	}*/

	ComponentHit->DestroyComponent(false);
	TArray<UBoxComponent*> Comps;
	GetComponents(UBoxComponent::StaticClass(), Comps, true);
	
	
	if (Comps.IsEmpty()) {
		AKoalaGameModeBase* GameMode = Cast<AKoalaGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
		GameMode->FireActorsInLevel--;
		UE_LOG(LogTemp, Warning, TEXT("Destroying FIRE"));
		GetWorldTimerManager().ClearTimer(DamageTimer);
		// GetWorldTimerManager().ClearTimer(FireSoundTimer);
		if (FireSoundComp) {
			FireSoundComp->SetActive(false, true);
		}
		GetWorldTimerManager().ClearTimer(SpreadTimer);
		GetWorldTimerManager().ClearTimer(CollisionUpdateTimer);
		GetWorldTimerManager().ClearAllTimersForObject(this);
		this->Destroy();
	}
}


// void AFire::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
void AFire::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// TODO Fix applydamage when moving into fire. Should stop applying directly but only trough the timer handler
	// const bool bIsAKoala = OtherActor->IsA(::StaticClass());
	if (OtherActor->IsA(ACharacter::StaticClass()) || OtherActor->IsA(ABaseTree::StaticClass()) || OtherActor->IsA(AConsumable::StaticClass())) {
		OverlapActors.Add(OtherActor);
	}
	if (OtherComp->GetOwner() == this) {
		DestroyFire(OverlappedComponent, true);
		return;
	}
	/*else if (AFire* OtherFire = Cast<AFire>(OtherActor)) {
		TArray<AActor*> Overlapss;
		OtherComp->GetOverlappingActors(Overlapss, ABaseTree::StaticClass());
		if (Overlapss.IsEmpty()) {
			OtherFire->DestroyFire(OverlappedComponent, true);
			return;
		}
	}*/
	if (ABaseTree* TreeObject = Cast<ABaseTree>(OtherActor)) {
		//InitializeSpline(TreeObject->SplineComponent);
		UE_LOG(LogTemp, Warning, TEXT("Overlapping TREE %s"), *OtherActor->GetName());
		InitializeSplines(TreeObject);
		/* if(TargetSpline)
		{
			GetWorldTimerManager().SetTimer(SplineTimer, this, &AFire::FollowSpline, 1.f, true, 0.f);
		} */
		//TreeObject->StartFire();
	}
	/*else {
		return;
	}*/
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
		//InitializeSpline(TreeObject->SplineComponent);
		InitializeSplines(TreeObject);
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

// void AFire::OnOverlapEnd(AActor* OverlappedActor, AActor* OtherActor)
void AFire::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
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

void AFire::MakeFire(FVector Location, FRotator Rotation)
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
	// NewBoxComp->SetHiddenInGame(false, true);
	NewBoxComp->bMultiBodyOverlap = true;

	NewBoxComp->OnComponentBeginOverlap.AddDynamic(this, &AFire::OnOverlapBegin);
	NewBoxComp->OnComponentEndOverlap.AddDynamic(this, &AFire::OnOverlapEnd);

	if (NewBoxComp->IsOverlappingActor(this)) {
		UE_LOG(LogTemp, Warning, TEXT("Destroying overllaped fire"));
		NewBoxComp->DestroyComponent();
		return;
	}
	UNiagaraComponent* NewNiagara = UNiagaraFunctionLibrary::SpawnSystemAttached(NiagaraSystemManual, NewBoxComp, NAME_None, FVector::ZeroVector, NiagaraParticleRotation, EAttachLocation::SnapToTargetIncludingScale, true);
	NewNiagara->SetRelativeScale3D(NiagaraParticleScale);
	NewNiagara->SetHiddenInGame(false, true);

	UpdateBoxCollisions();
	// UE_LOG(LogTemp, Warning, TEXT("Children of %s: %d"), *GetName(), GetComponents().Num());
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
		GetWorldTimerManager().ClearTimer(SpreadTimer);
		GetWorldTimerManager().ClearTimer(DamageTimer);
		// GetWorldTimerManager().ClearTimer(FireSoundTimer);
		return;
	}
	if (KoalaBasePlayer->IsDead()) {
		GetWorldTimerManager().ClearTimer(SpreadTimer);
		GetWorldTimerManager().ClearTimer(DamageTimer);
		// GetWorldTimerManager().ClearTimer(FireSoundTimer);
		return;
	}
	for (int i = 0; i < Num; i++) {
		if (!OverlapActors.IsValidIndex(i) || OverlapActors[i] == nullptr) {
			OverlapActors.Empty();
			GetWorldTimerManager().ClearTimer(SpreadTimer);
			GetWorldTimerManager().ClearTimer(DamageTimer);
			// GetWorldTimerManager().ClearTimer(FireSoundTimer);
			return;
		}
		// UE_LOG(LogTemp, Warning, TEXT("%s"), *Actor->GetName());
		UGameplayStatics::ApplyDamage(OverlapActors[i], TickDamage, KoalaBasePlayer->GetController(), KoalaBasePlayer, UDamageType::StaticClass());
	}
	
}


void AFire::TrunkSplineHandle(class USplineComponent* SplineComponent)
{

	FVector Start = SplineComponent->GetLocationAtTime(0, ESplineCoordinateSpace::World, true);
	FVector End = SplineComponent->GetLocationAtTime(SplineComponent->Duration, ESplineCoordinateSpace::World, true);
	float Length = End.Z - Start.Z;
	float ActorsNeeded = Length / CollisionBoxExtent;

	float Delay = SplineComponent->Duration / ActorsNeeded;
	// UE_LOG(LogTemp, Warning, TEXT("Length %f ActorsNeeded %f Delay %f"), Length, ActorsNeeded, Delay);

	for(int i=1; i<=ActorsNeeded;i++)
	{
		CalculateSplineLocations(SplineComponent, SplineLocations, Delay * i);
	}

	
	GetWorldTimerManager().SetTimer(SplineTimer,this, &AFire::SpawnTrunkActors, SpawnOnTreeDelay, true);

}

void AFire::BranchSplineHandle(class USplineComponent* SplineComponent)
{
	float Length = SplineComponent->GetSplineLength();
	FVector Start = SplineComponent->GetLocationAtTime(0, ESplineCoordinateSpace::World, true);
	FVector End = SplineComponent->GetLocationAtTime(SplineComponent->Duration, ESplineCoordinateSpace::World, true);
	// UE_LOG(LogTemp, Warning, TEXT("Branch Length: %f | Calculated Length: %s"), Length, *(End - Start).ToString());
	// Output: Branch Length: 240.000000 | Calculated Length: X=0.000 Y=240.000 Z=0.000. This means it is equal to length
	float ActorsNeeded = Length / CollisionBoxExtent;

	float Delay = SplineComponent->Duration / ActorsNeeded;

	for(int i=1; i<=ActorsNeeded;i++)
	{
		/* TODO: fix to have different different indexes for different branches
		1. zindex?
		*/
		CalculateSplineLocations(SplineComponent,BranchLocations, Delay * i);
	}
		
	// GetWorld()->GetTimerManager().SetTimer(BranchTimerHandle,this, &AFire::SpawnBranchActors, SpawnOnTreeDelay, true);
}




void AFire::InitializeSplines(ABaseTree *Tree)
{
	bool bTreeHasBranches = Tree->BranchesSplinesComponent.Num() > 0;
	bool bTreeHasTrunkSpline = Tree->SplineComponent != nullptr;

	if (bTreeHasBranches)
	{
		/* TODO: Logic for branches
		1. check when fire reaches Z axis of branch (should look if branch.Z is +- Fire.Z)
		2. when reaches, start new timer for branch fire
		*/
		for (TTuple<USplineComponent*, float> Tuple : Tree->BranchesSplinesComponent)
		{
			USplineComponent* Branch = Tuple.Get<0>();
			BranchSplineHandle(Branch);
		}
	}

	if (bTreeHasTrunkSpline)
	{
		TrunkSplineHandle(Tree->SplineComponent);
	}


	

}
	

void AFire::CalculateSplineLocations(USplineComponent *Spline, TArray<TTuple<FVector, FRotator>>& OutLocations, float Time)
{
	FVector SplineLocation = Spline->GetLocationAtTime(Time, ESplineCoordinateSpace::World, true);
	FRotator SplineRotation = Spline->GetRotationAtTime(Time, ESplineCoordinateSpace::World);

	OutLocations.Add(TTuple<FVector, FRotator>(SplineLocation, SplineRotation));
}

void AFire::SpawnTrunkActors()
{
	FVector LastUsedLocation = SpawnSplineActors(SplineLocations, SplineTimer);
	bool bTreeHasBranches = BranchLocations.Num() > 0;
	if (!bTreeHasBranches)
	{
		return;
	}
	bool bIsBranchLower = BranchLocations[0].Get<0>().Z < LastUsedLocation.Z;
	UE_LOG(LogTemp, Warning, TEXT("BranchLocations[0].Get<0>().Z: %f - LastUsedLocation.Z: %f"), BranchLocations[0].Get<0>().Z, LastUsedLocation.Z);
	if (bIsBranchLower)
	{
		UE_LOG(LogTemp, Warning, TEXT("bIsBranchLower entered"));
		if (!GetWorld()->GetTimerManager().IsTimerActive(BranchTimerHandle))
		{
			GetWorld()->GetTimerManager().SetTimer(BranchTimerHandle,this, &AFire::SpawnBranchActors, SpawnOnTreeDelay, true);
		}
	}
	
}

void AFire::SpawnBranchActors()
{
	UE_LOG(LogTemp, Warning, TEXT("SpawnBranchActors called"));
	SpawnSplineActors(BranchLocations, BranchTimerHandle);
}

FVector AFire::SpawnSplineActors(TArray<TTuple<FVector, FRotator>>& OutLocations, FTimerHandle& ClearTimer)
{
	// UE_LOG(LogTemp, Warning, TEXT("Inside here locations num: %d"), OutLocations.Num());
	if(OutLocations.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("Deleting"));

		GetWorldTimerManager().ClearTimer(ClearTimer);
		return FVector::ZeroVector;
	}

	TTuple<FVector, FRotator> Location = OutLocations[0];
	
	// UE_LOG(LogTemp, Warning, TEXT("%s %s"), *Location.Get<0>().ToString(), *Location.Get<1>().ToString());
	
	AFire* NewFire = GetWorld()->SpawnActor<AFire>(FireClass, Location.Get<0>(), Location.Get<1>());
	if(NewFire)
	{
		NewFire->SetActorTickEnabled(false);
		// NewFire->SetActorEnableCollision(true);
		// 
		// NewFire->SetActorEnableCollision(false);
	}
	// MakeFire(Location.Get<0>(), Location.Get<1>());
	UE_LOG(LogTemp, Warning, TEXT("SPAWNING TREE FIRE"));
	OutLocations.RemoveAt(0);

	return Location.Get<0>();
}
