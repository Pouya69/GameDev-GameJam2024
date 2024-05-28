// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "KoalaBaseCharacter.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath);
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

public:
	// Death
	// Delegate signature
	UFUNCTION(BlueprintCallable)
		void Die();

	FOnDeath& OnDeath() { return DeathEvent; }

	

	UCapsuleComponent* CapsuleComp;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Interaction | Trees")
		float TreeDistanceCheck = 100;
	bool AreThereAnyTreesAround(FHitResult& OutHitResult) const;
	bool GetObjectAround(FHitResult& OutHitResult, float RangeCheck) const;

	UFUNCTION(BlueprintCallable)
		bool IsOnFire() const;

public:
	// Stamina and Health
	UFUNCTION(BlueprintCallable)
		bool IsCharacterMoving() const;
	UFUNCTION(BlueprintCallable, Category = "Stamina")
		float GetStamina() const { return Stamina; }
	UPROPERTY(EditAnywhere, Category = "Stamina")
		float StaminaDeductionRate = 5.f;
	UPROPERTY(EditAnywhere, Category = "Stamina")
		float StaminaDeductionMultiplierMoving = 1.3; // While moving it will be deducted 1.3X for example. Idle will be the same.
	UPROPERTY(EditAnywhere, Category = "Stamina")
		float MovementCheckSpeed = 20.f;  // This is for checking if the player is moving
	UPROPERTY(EditAnywhere, Category = "Stamina")
		float SleepDelay = 3.f;
	UPROPERTY(EditAnywhere, Category = "Stamina")
		float StaminaAfterSleep = 50.f;
	UPROPERTY(EditAnywhere, Category = "Stamina")
		bool bIsBeingCarried;
	UFUNCTION(BlueprintCallable)
		float GetHealth() const { return Health; }
	/* UFUNCTION()
		void DamageTakenHandle(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser); */
	UFUNCTION()
		virtual float TakeDamage(float Damage, struct FDamageEvent const &DamageEvent,  class AController* InstigatedBy, AActor* DamageCauser) override;
	UFUNCTION(BlueprintCallable)
		void ConsumeItem(class AConsumable* Eucalyptus);
	UFUNCTION(BlueprintCallable)
		void AddHealth(float Amount);
	UFUNCTION(BlueprintCallable)
		void AddStamina(float Amount);
	UFUNCTION(BlueprintCallable)
		void Sleep();
	UFUNCTION(BlueprintCallable)
		bool IsSleeping() const { return bIsSleeping; }
	UFUNCTION(BlueprintCallable)
		bool IsDead() const { return bIsDead; }
	
	FTimerHandle SleepTimerHandle;



private:
	/** Broadcasts whenever the layer changes */
	FOnDeath DeathEvent;

	float Health = 100;
	float Stamina = 100;
	bool bIsSleeping;
	bool bIsDead;
};



