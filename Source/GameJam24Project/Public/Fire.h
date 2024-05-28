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
	float SpreadTime = 3.f;

	UPROPERTY(EditAnywhere, Category="Fire Properties")
	float CollisionBoxExtent = 50.f;

	UPROPERTY(EditAnywhere, Category="Fire Properties")
	FVector NiagaraParticleScale = FVector(7.f,7.f,4.f);

	UPROPERTY(EditAnywhere, Category="Fire Properties")
	float NiagaraParticleZOffset = -30.f;

	UPROPERTY(EditAnywhere, Category="Fire Properties")
	class UNiagaraSystem* NiagaraTemplate;

	UPROPERTY(EditAnywhere)
	class UBoxComponent* BoxComponent;

	UPROPERTY(EditAnywhere)
	class UNiagaraComponent* Niagara;

	

	void SpreadFire();

	void ApplyDamageTimer();

	void SpawnFire(FVector Location);

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& HitResult);

private:

	FTimerHandle SpreadTimer;
	FTimerHandle DamageTimer;

	TArray<FVector> SpreadDirections;

	UPROPERTY(EditAnywhere, Category="Custom Properties")
	float SpawnProbability = 5.f;
	
	UPROPERTY(EditAnywhere, Category="Custom Properties")
	float IncrementProbabilityRate = 5.f;

};