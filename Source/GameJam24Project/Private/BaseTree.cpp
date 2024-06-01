// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseTree.h"
#include "Consumable.h"

ABaseTree::ABaseTree()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ConsumableSpawnLocation = CreateDefaultSubobject<USceneComponent>(FName("Consumable Spawn Location 1"));
	ConsumableSpawnLocation2 = CreateDefaultSubobject<USceneComponent>(FName("Consumable Spawn Location 2"));
	ConsumableSpawnLocation->SetupAttachment(BaseMeshComp);
	ConsumableSpawnLocation2->SetupAttachment(BaseMeshComp);
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
	/*if (bIsOnFire) return;  // If already on fire ignore
	bIsOnFire = true;
	FTimerDelegate TreeFireDelegate;
	TreeFireDelegate.BindLambda([&]() {
		// TODO: Expand fire and create fire objects and etc.
		// TODO: Also have an end condition when the tree is fully on fire
	});

	GetWorldTimerManager().SetTimer(TreeFireTimer, TreeFireDelegate, ExpandFireEverySeconds, true);*/

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

