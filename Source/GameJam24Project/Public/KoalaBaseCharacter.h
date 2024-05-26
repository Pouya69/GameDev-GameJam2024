// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "KoalaBaseCharacter.generated.h"

UCLASS()
class GAMEJAM24PROJECT_API AKoalaBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AKoalaBaseCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UCapsuleComponent* CapsuleComp;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Interaction | Trees")
		float TreeDistanceCheck = 100;
	bool AreThereAnyTreesAround(FHitResult& OutHitResult) const;
	bool GetObjectsAround(FHitResult& OutHitResult, float RangeCheck) const;


private:
	

};



