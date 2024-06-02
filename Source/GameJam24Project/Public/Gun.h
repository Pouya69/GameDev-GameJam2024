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
	void ReleaseTrigger();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "Gun Properties")
	USoundBase* ShootSound;

	UPROPERTY(EditAnywhere, Category = "Gun Properties")
	USoundBase* HitSound;

	UPROPERTY(EditAnywhere, Category = "Gun Properties")
	USoundBase* FireHitSound;

	UPROPERTY(EditAnywhere, Category = "Gun Properties")
	USoundBase* RefillSound;

	UPROPERTY(EditAnywhere, Category = "Gun Properties")
	USoundBase* StopShootSound;



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
	class UNiagaraComponent* Niagara;

	UPROPERTY(EditAnywhere)
	class UNiagaraComponent* NiagaraEnd;

	UPROPERTY(EditAnywhere)
	class UNiagaraSystem* NiagaraHitObject;

	UPROPERTY(EditAnywhere)
	class UNiagaraSystem* NiagaraHitFire;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* GunBeamMesh;

	UPROPERTY(EditAnywhere)
	class USceneComponent* Root;

	UPROPERTY(EditAnywhere)
	FVector NotHittingScale3D;

	FRotator InitialRotationBeam;

	float GetAmmoLeft() const { return Ammunition; }

	UFUNCTION()
	void ReloadAmmunition();

	class AKoalaPlayerCharacter* PlayerCharacter;

private:
	bool GunTrace(FHitResult& Hit);
	float Ammunition = AmmunitionCapacity;

	void SpawnEmitterJetEffect(const FVector& Start, const FVector& End);
	
	

};
