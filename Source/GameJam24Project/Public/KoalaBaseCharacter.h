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
	bool GetObjectAround(FHitResult& OutHitResult, float RangeCheck) const;

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
	UPROPERTY(EditAnywhere, Category = "Health")
		float TimeBetweenDamage = 1.f;
	UFUNCTION(BlueprintCallable)
		float GetHealth() const { return Health; }
	/* UFUNCTION()
		void DamageTakenHandle(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser); */
	UFUNCTION()
		virtual float TakeDamage(float Damage, struct FDamageEvent const &DamageEvent,  class AController* InstigatedBy, AActor* DamageCauser) override;
	UFUNCTION(BlueprintCallable)
		void Die();
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
	FTimerHandle SleepTimerHandle;



private:
	float Health = 100;
	float Stamina = 100;
	float LastDamageTime = 0;
	bool bIsSleeping;

};



