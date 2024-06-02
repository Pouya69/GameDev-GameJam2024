// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Fire.generated.h"

UCLASS()
class GAMEJAM24PROJECT_API AFire : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFire();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;



	UPROPERTY(EditAnywhere, Category="Fire Properties")
	float TickDamage = 10.f;

	UPROPERTY(EditAnywhere, Category="Fire Properties")
	float SpreadTime = FMath::RandRange(3,10);

	UPROPERTY(EditAnywhere, Category="Fire Properties")
	float SplineTime = 3.f;

	UPROPERTY(EditAnywhere, Category="Fire Properties")
	float SplineDelay;


	UPROPERTY(EditAnywhere, Category="Fire Properties")
	float CollisionBoxExtent = 50.f;

	UPROPERTY(EditAnywhere, Category="Fire Properties")
	FVector NiagaraParticleScale = FVector(7.f,7.f,4.f);

	UPROPERTY(EditAnywhere, Category = "Fire Properties")
	FRotator NiagaraParticleRotation;

	UPROPERTY(EditAnywhere, Category="Fire Properties")
	float NiagaraParticleZOffset = -30.f;

	UPROPERTY(EditAnywhere, Category="Fire Properties")
	TSubclassOf<AFire>  FireClass;

	UPROPERTY(EditAnywhere, Category = "Fire Properties")
	TSubclassOf<class ABaseTree>  TreeClass;

	UPROPERTY(EditAnywhere, Category = "Fire Properties")
	float FireCreationRadius = 100.f;

	UPROPERTY(EditAnywhere, Category = "Fire Properties")
	float FireCreationUpwardsCheck = 50.f;

	TArray<TTuple<FVector, FRotator>> SplineLocations;
	TArray<TTuple<FVector, FRotator>> BranchLocations;

	UPROPERTY(EditAnywhere)
	class USceneComponent* SceneRootComp;

	UPROPERTY(EditAnywhere)
	class UNiagaraSystem* NiagaraSystemManual;

	void SpreadFire();

	bool GetRandomLocation(FVector& OutLocation);

	void ApplyDamageTimer();

	void SpawnFire(FVector Location);

	void DestroyFire(UPrimitiveComponent* ComponentHit);

	UFUNCTION()
	void OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION()
	void OnOverlapEnd(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION()
	virtual float TakeDamage(float Damage, struct FDamageEvent const &DamageEvent,  class AController* InstigatedBy, AActor* DamageCauser) override;

	void UpdateBoxCollisions();
	void CheckAndDestroyConsumable(class AConsumable* Consumable);

	void TrunkSplineHandle(class USplineComponent* SplineComponent);
	void BranchSplineHandle(class USplineComponent* SplineComponent);
	void InitializeSplines(class ABaseTree* Tree);

private:
	void MakeFire(FVector Location);
	bool bIsCheckingOnTree = false;
	FVector LocationToSpawnFrom;
	FTimerHandle SpreadTimer;
	FTimerHandle DamageTimer;
	FTimerHandle SplineTimer;
	FTimerHandle CollisionUpdateTimer;
	FTimerHandle BranchTimerHandle;
	TArray<AActor*> OverlapActors;

	class USplineComponent* TargetSpline;

	UPROPERTY(EditAnywhere, Category="Fire Properties")
	float SpawnProbability = 20.f;
	
	UPROPERTY(EditAnywhere, Category="Fire Properties")
	float IncrementProbabilityRate = 5.f;

	UPROPERTY(EditAnywhere, Category="Fire Properties")
	float SpawnOnTreeDelay = 1.f;

	void CalculateSplineLocations(USplineComponent* Spline, TArray<TTuple<FVector, FRotator>>& OutLocations, float Time);
	void SpawnTrunkActors();
	void SpawnBranchActors();
	FVector SpawnSplineActors(TArray<TTuple<FVector, FRotator>>& OutLocations, FTimerHandle& ClearTimer);

};