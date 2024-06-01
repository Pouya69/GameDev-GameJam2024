// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseTree.h"
#include "Consumable.h"
#include "Components/SplineComponent.h"

ABaseTree::ABaseTree()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ConsumableSpawnLocation = CreateDefaultSubobject<USceneComponent>(FName("Consumable Spawn Location 1"));
	ConsumableSpawnLocation2 = CreateDefaultSubobject<USceneComponent>(FName("Consumable Spawn Location 2"));
	SplineComponent = CreateDefaultSubobject<USplineComponent>(FName("Spline Component"));
	ConsumableSpawnLocation->SetupAttachment(BaseMeshComp);
	ConsumableSpawnLocation2->SetupAttachment(BaseMeshComp);
	SplineComponent->SetupAttachment(BaseMeshComp);

	SplineComponent->ClearSplinePoints();
	SplineComponent->AddSplinePoint(FVector::ZeroVector, ESplineCoordinateSpace::Local);
	SplineComponent->AddSplinePoint(FVector(0,0,500), ESplineCoordinateSpace::Local);
	
	
}

// Called when the game starts or when spawned
void ABaseTree::BeginPlay()
{
	Super::BeginPlay();

	for (int32 i = 0; i < BranchNumber; i++)
	{
		if(USplineComponent* BranchSplineComponent = Cast<USplineComponent>(GetDefaultSubobjectByName(*FString::Printf(TEXT("Branch%d"), i))))
		{
			FVector Location = BranchSplineComponent->GetLocationAtTime(0, ESplineCoordinateSpace::World, true);
			/* TODO: TArray creation logic
			1. Not sure if Z calculation should go here or in fire.
			*/
			BranchesSplinesComponent.Add(TTuple<USplineComponent*, float>(BranchSplineComponent, Location.Z));
		}
		
	}
	
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

bool ABaseTree::AlreadyHasConsumableOnTree() const
{
	bool bHasConsumable = false;
	FHitResult HitResult;
	const FVector Loc1 = ConsumableSpawnLocation->GetComponentLocation();
	const FCollisionShape CollisionShapeCheck = FCollisionShape::MakeSphere(ConsumableExistCheckRadius);
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	bHasConsumable = GetWorld()->SweepSingleByChannel(HitResult, Loc1, Loc1, FQuat::Identity, ECollisionChannel::ECC_GameTraceChannel1, CollisionShapeCheck, Params);
	if (bHasConsumable) {
		if (HitResult.GetActor()->IsA(AConsumable::StaticClass())) {
			return true;
		}
	}
	const FVector Loc2 = ConsumableSpawnLocation2->GetComponentLocation();
	bHasConsumable = GetWorld()->SweepSingleByChannel(HitResult, Loc2, Loc2, FQuat::Identity, ECollisionChannel::ECC_GameTraceChannel1, CollisionShapeCheck, Params);
	if (bHasConsumable) {
		if (HitResult.GetActor()->IsA(AConsumable::StaticClass())) {
			return true;
		}
	}
	return false;
}

void ABaseTree::SpawnConsuamble()
{
	FTransform SpawnTransform = FMath::RandBool() ? ConsumableSpawnLocation->GetComponentTransform() : ConsumableSpawnLocation2->GetComponentTransform();
	SpawnTransform.SetScale3D(FVector(1, 1, 1));
	FActorSpawnParameters SpawnParams;
	AConsumable* ConsumableSpawned = GetWorld()->SpawnActor<AConsumable>(ConsumableClass, SpawnTransform);
	ConsumableSpawned->BaseMeshComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	ConsumableSpawned->BaseMeshComp->SetSimulatePhysics(false);
	ConsumableSpawned->BaseMeshComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

