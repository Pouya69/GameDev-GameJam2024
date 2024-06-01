// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseInteractableObject.h"
#include "BaseTree.generated.h"

/**
 * 
 */
UCLASS()
class GAMEJAM24PROJECT_API ABaseTree : public ABaseInteractableObject
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	ABaseTree();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Interact() override;


public:
	// Fire handling
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Fire")
		bool bIsOnFire;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Fire")
		int ExpandFireEverySeconds;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Fire")
		int TreeFireDamage;

	UPROPERTY(EditAnywhere, Category = "Spline")
		class USplineComponent* SplineComponent;

	UPROPERTY(EditAnywhere, Category = "Spline")
		int BranchNumber;
	
	
	TArray<TTuple<USplineComponent*, float>> BranchesSplinesComponent;
		


	void StartFire();
	FTimerHandle TreeFireTimer;

public:
	// Consumable handling
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Consumable")
		bool bCanSpawnFruit;  // For if a tree that cannot spawn a tree
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Consumable")
		float ConsumableExistCheckRadius = 100;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Consumable")
		TSubclassOf<class AConsumable> ConsumableClass;  // The type of consumable spawns in BP
	UPROPERTY(EditAnywhere, Category = "Consumable")
		USceneComponent* ConsumableSpawnLocation;  // The type of consumable spawns in BP
	UPROPERTY(EditAnywhere, Category = "Consumable")
		USceneComponent* ConsumableSpawnLocation2;  // The type of consumable spawns in BP

	bool AlreadyHasConsumableOnTree() const;
	void SpawnConsuamble();
};
