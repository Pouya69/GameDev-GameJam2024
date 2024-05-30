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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Fire")
		bool bIsOnFire;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Fire")
		int ExpandFireEverySeconds;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Fire")
		int TreeFireDamage;


	void StartFire();
	FTimerHandle TreeFireTimer;
	
	
};
