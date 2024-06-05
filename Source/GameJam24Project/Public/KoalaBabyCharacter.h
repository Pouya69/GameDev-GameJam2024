// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KoalaBaseCharacter.h"
#include "KoalaBabyCharacter.generated.h"

/**
 * 
 */
UCLASS()
class GAMEJAM24PROJECT_API AKoalaBabyCharacter : public AKoalaBaseCharacter
{
	GENERATED_BODY()

public:
	AKoalaBabyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void Die() override;

	UPROPERTY(EditAnywhere, Category = "Player Clue")
		UMaterialInterface* NormalMaterial;
};
