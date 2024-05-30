// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gun.generated.h"

UCLASS()
class GAMEJAM24PROJECT_API AGun : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGun();

	void PullTrigger();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category="Gun Properties")
	class USkeletalMeshComponent* MeshComponent;
	
	UPROPERTY(EditAnywhere, Category="Gun Properties")
	float FireRange = 1000.f;

	UPROPERTY(EditAnywhere, Category="Gun Properties")
	float Damage = 50.f;

	UPROPERTY(EditAnywhere, Category="Gun Properties")
	float AmmunitionCapacity = 1000.f;

	UPROPERTY(EditAnywhere, Category="Gun Properties")
	float AmmoConsumeRate = 1.f;


	UPROPERTY(EditAnywhere)
	class UNiagaraSystem* Niagara;

	UPROPERTY(EditAnywhere)
	class USceneComponent* Root;

	UFUNCTION()
	void ReloadAmmunition();

private:
	AController* GetOwnerController() const;
	bool GunTrace(FHitResult& Hit);
	float Ammunition = AmmunitionCapacity;

	void SpawnEmitterJetEffect(const FVector& Start, const FVector& End);
	

};
