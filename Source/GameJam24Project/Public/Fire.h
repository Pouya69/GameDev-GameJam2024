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
	float SpreadTime = 3.f;

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

private:
	void MakeFire(FVector Location);
	bool bIsCheckingOnTree = false;
	FVector LocationToSpawnFrom;
	FTimerHandle SpreadTimer;
	FTimerHandle DamageTimer;
	FTimerHandle CollisionUpdateTimer;
	TArray<AActor*> OverlapActors;

	UPROPERTY(EditAnywhere, Category="Fire Properties")
	float SpawnProbability = 20.f;
	
	UPROPERTY(EditAnywhere, Category="Fire Properties")
	float IncrementProbabilityRate = 5.f;

};