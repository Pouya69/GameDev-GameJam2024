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

	UPROPERTY(EditAnywhere, Category="Custom Properties")
	float SpreadTime = 3.f;

	UPROPERTY(EditAnywhere, Category="Custom Properties")
	float CollisionBoxExtent = 50.f;

	UPROPERTY(EditAnywhere)
	class UBoxComponent* BoxComponent;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Mesh;

	void SpreadFire();

	void SpawnFire(FVector Location);

private:

	FTimerHandle SpreadTimer;

	TArray<FVector> SpreadDirections;

	UPROPERTY(EditAnywhere, Category="Custom Properties")
	float SpawnProbability = 5.f;
	
	UPROPERTY(EditAnywhere, Category="Custom Properties")
	float IncrementProbabilityRate = 5.f;

};