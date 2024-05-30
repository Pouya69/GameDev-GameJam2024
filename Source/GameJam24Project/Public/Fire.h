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

	UPROPERTY(EditAnywhere, Category="Fire Properties")
	float NiagaraParticleZOffset = -30.f;

	UPROPERTY(EditAnywhere, Category="Fire Properties");
	TSubclassOf<AFire>  FireClass;

	UPROPERTY(EditAnywhere)
	class UBoxComponent* BoxComponent;

	UPROPERTY(EditAnywhere)
	class UNiagaraComponent* Niagara;

	

	void SpreadFire();

	void ApplyDamageTimer();

	void SpawnFire(FVector Location, TArray<FVector>& HitVector);

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& HitResult);

	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	virtual float TakeDamage(float Damage, struct FDamageEvent const &DamageEvent,  class AController* InstigatedBy, AActor* DamageCauser) override;

private:

	FTimerHandle SpreadTimer;
	FTimerHandle DamageTimer;
	AActor* ActorToDamage;
	TArray<FVector> SpreadDirections;
	bool bIsOverlapping = false;

	UPROPERTY(EditAnywhere, Category="Fire Properties")
	float SpawnProbability = 5.f;
	
	UPROPERTY(EditAnywhere, Category="Fire Properties")
	float IncrementProbabilityRate = 5.f;

};